// Copyright (c) 2011 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#include "shared/database.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <glog/logging.h>
#include <boost/unordered_map.hpp>

#include "parser/group.h"
#include "proto/tick.pb.h"
#include "shared/dataset.h"
#include "util/util.h"

#include <boost/regex.hpp>

DataBase::DataBase(std::string data_dir) {
  VLOG(1) << "initializing database from " << data_dir;
  data_dir_ = data_dir;
}

DataBase::~DataBase() {
}

std::string DataBase::computeFileName(TimeStamp& day, int seconds_per_unit) {
  std::stringstream stream;
  stream << data_dir_ << "/" << day.ToString(2) << ".quote."
      << seconds_per_unit;
  return stream.str();
}

void DataBase::ReadDay(TimeStamp& day, int seconds_per_unit,
    std::string pattern, DataSet& dataset) {

  dataset.SetStartTime(day);
  dataset.SetSecondsPerUnit(seconds_per_unit);

  std::string file_name = computeFileName(day, seconds_per_unit);

  std::fstream fin;
  fin.open(file_name.c_str(), std::ios::in | std::ios::binary);

  if (!fin.good()) {
    LOG(ERROR) << "could not read file " << file_name << "\n";
    exit(1);
  }

  int size;
  data::Group *group = new data::Group();

  // TODO(hagonzal): Guarantee a safe size for the buffer
  char *buf = new char[1024 * 100];

  int n = 0;

  boost::regex e(pattern);

  while (!fin.eof()) {
    fin.read((char *)&size, sizeof(int));
    if (fin.eof()) {
      continue;
    }

    fin.read(buf, size);

    group->ParseFromArray(buf, size);

    if (!boost::regex_match(group->symbol(), e)) {
      continue;
    }

    n = group->values_size();

    double *bid = new double[n];
    double *ask = new double[n];
    double *bid_size = new double[n];
    double *ask_size = new double[n];

    for (int i = 0; i < n; i++) {
      data::TickValue v = group->values(i);
      bid[i] = v.bid();
      ask[i] = v.ask();
      bid_size[i] = v.bid_size();
      ask_size[i] = v.ask_size();
    }

    dataset.AddTimeSeries(group->symbol() + "_b", bid);
    dataset.AddTimeSeries(group->symbol() + "_bs", bid_size);
    dataset.AddTimeSeries(group->symbol() + "_a", ask);
    dataset.AddTimeSeries(group->symbol() + "_as", ask_size);
  }
  dataset.SetDimension(n);

  delete[] buf;
  delete group;
  fin.close();
}
