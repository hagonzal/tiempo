// Copyright (c) 2012 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#include "shared/dataset.h"

#include <fcntl.h>
#include <stdio.h>
#include <time.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <boost/regex.hpp>
#include <boost/unordered_map.hpp>

#include "parser/group.h"
#include "proto/tick.pb.h"

DataSet::DataSet() {
}

DataSet::~DataSet() {
  Clear();
}

void DataSet::Clear() {
  std::map<std::string, double *>::iterator it = data_.begin();

  while (it != data_.end()) {
    double *values = (*it).second;
    delete[] values;
    it++;
  }
  data_.clear();
  dimension_ = 0;
}

void DataSet::FillFrom(const DataSet& copy_from_me) {
  Clear();

  std::map<std::string, double *>::const_iterator it
      = copy_from_me.data_.begin();
  while (it != copy_from_me.data_.end()) {
    double *d = new double[copy_from_me.dimension_];
    memcpy(d, it->second, copy_from_me.dimension_ * sizeof(double));

    AddTimeSeries(it->first, d);
    it++;
  }
  dimension_ = copy_from_me.dimension_;
  start_time_ = copy_from_me.start_time_;
  seconds_per_unit_ = copy_from_me.seconds_per_unit_;
}

void DataSet::Print() {
  Print(".*", NULL);
}

void DataSet::Print(const std::string& pattern, double *valid) {
  std::map<std::string, double *>::iterator it;

  boost::regex e(pattern);

  // Print the header
  it = data_.begin();

  std::string sep = "";
  while (it != data_.end()) {
    std::string key = (*it).first;
    if (!boost::regex_match(key, e)) {
      it++;
      continue;
    }

    std::cout << sep << key;
    it++;

    sep = ",";
  }
  std::cout << "\n";

  for (int i = 0; i < dimension_; i++) {
    sep = "";
    it = data_.begin();
    bool ok = false;
    while (it != data_.end()) {
      std::string key = (*it).first;
      if (!boost::regex_match(key, e)) {
        it++;
        continue;
      }

      double *values = (*it).second;

      if (valid == NULL || valid[i]) {
        std::cout << sep << values[i];
        sep = ",";
        ok = true;
      }
      it++;
    }
    if (ok) {
      std::cout << "\n";
    }
  }
}

void DataSet::ComputeOr(const std::string& n1, const std::string& n2,
    double *r) {
  int n = GetDimension();

  double *v1 = GetTimeSeries(n1);
  double *v2 = GetTimeSeries(n2);

  for (int i=0;i<n;i++) {
    bool a = v1[i];
    bool b = v2[i];

    r[i] = (a | b ? 1.0 : 0.0);
  }
}

void DataSet::ComputeAnd(const std::string& n1, const std::string& n2,
    double *r) {
  int n = GetDimension();

  double *v1 = GetTimeSeries(n1);
  double *v2 = GetTimeSeries(n2);

  for (int i=0;i<n;i++) {
    bool a = v1[i];
    bool b = v2[i];

    r[i] = (a & b ? 1.0 : 0.0);
  }
}

double *DataSet::GetTimeSeries(const std::string& name) {
  return data_[name];
}

void DataSet::AddTimeSeries(const std::string& name, double *values) {
  data_[name] = values;
}

void DataSet::RemoveTimeSeries(const std::string& name) {
  std::map<std::string, double *>::iterator it;
  it = data_.find(name);
  if (it != data_.end()) {
    double *values = (*it).second;
    delete[] values;
    data_.erase(it);
  }
}

std::string DataSet::ComputePrefix(const std::string& a) {
  int n = a.find("_", 0);
  if (n < 0) {
    return a;
  }

  return a.substr(0, n);
}

std::string DataSet::ComputeSuffix(const std::string& a) {
  int n = a.find("_", 0);
  if (n < 0) {
    return a;
  }

  return a.substr(n + 1);
}

int DataSet::GetDimension() {
  return dimension_;
}

void DataSet::SetDimension(int n) {
  dimension_ = n;
}

void DataSet::GetBaseNames(std::vector<std::string>& result) {
  std::map<std::string, double *>::iterator it;
  it = data_.begin();

  std::string prev = "";
  while (it != data_.end()) {
    std::string key = (*it).first;

    std::string prefix = ComputePrefix(key);
    if (prefix != prev) {
      result.push_back(prefix);
      prev = prefix;
    }
    it++;
  }
}

void DataSet::DeleteSuffix(const std::string& suffix) {
  std::map<std::string, double *>::iterator it;
  it = data_.begin();

  while (it != data_.end()) {
    std::string key = (*it).first;
    std::string s = ComputeSuffix(key);
    if (s == suffix) {
      delete[] (*it).second;
      data_.erase(it);
    }
    it++;
  }
}

void DataSet::DeletePrefix(const std::string& prefix) {
  std::map<std::string, double *>::iterator it;
  it = data_.begin();

  while (it != data_.end()) {
    std::string key = (*it).first;
    std::string s = ComputePrefix(key);
    if (s == prefix) {
      delete[] (*it).second;
      data_.erase(it);
    }
    it++;
  }
}

void DataSet::Concatenate(const DataSet& other) {
  std::map<std::string, double *>::iterator it1 = data_.begin();
  std::map<std::string, double *>::const_iterator it2 = other.data_.begin();

  int n = 0;
  while (it1 != data_.end() && it2 != other.data_.end()) {
    std::string key1 = it1->first;
    std::string key2 = it2->first;

    if (key1 < key2) {
      delete (*it1).second;
      data_.erase(it1++);

      continue;
    }

    if (key1 > key2) {
      it2++;
      continue;
    }

    // only union the entries that share a key
    double *d1 = it1->second;
    double *d2 = it2->second;

    n = dimension_ + other.dimension_;
    double *u = new double[n];

    memcpy(u, d1, sizeof(double) * dimension_);
    memcpy(u + dimension_, d2, sizeof(double) * other.dimension_);

    it1->second = u;

    delete d1;

    it1++;
    it2++;
  }
  dimension_ = n;
}

void DataSet::SetStartTime(const TimeStamp& timestamp) {
  start_time_ = timestamp;
}

TimeStamp DataSet::GetStartTime() {
  return start_time_;
}

void DataSet::SetSecondsPerUnit(int seconds_per_unit) {
  seconds_per_unit_ = seconds_per_unit;
}

int DataSet::GetSecondsPerUnit() {
  return seconds_per_unit_;
}

std::map<std::string, double *>::const_iterator DataSet::Begin() {
  return data_.begin();
}

std::map<std::string, double *>::const_iterator DataSet::End() {
  return data_.end();
}
