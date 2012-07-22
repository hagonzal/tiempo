// Copyright (c) 2012 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#include "regression/regression_set.h"

#include <iostream>
#include <fstream>

#include <glog/logging.h>

#include "regression/regression_result.h"
#include "proto/model.pb.h"

RegressionSet::RegressionSet() {
}

RegressionSet::~RegressionSet() {
  Clear();
}

RegressionResult * RegressionSet::GetInternal(std::string symbol) {
  boost::unordered_map<std::string, RegressionResult *>::iterator it;
  it = data_.find(symbol);
  if (it == data_.end()) {
    return NULL;
  }
  return it->second;
}

void RegressionSet::Add(RegressionResult *r) {
  RegressionResult *old = GetInternal(r->GetName());
  if (old != NULL) {
    delete old;
  }

  data_[r->GetName()] = r;
}

RegressionResult *RegressionSet::Get(std::string name) {
  return GetInternal(name);
}

void RegressionSet::Clear() {
  boost::unordered_map<std::string, RegressionResult *>::iterator it;
  it = data_.begin();

  while (it != data_.end()) {
    RegressionResult *r = it->second;
    delete r;
    it++;
  }
  data_.clear();
}

boost::unordered_map<std::string, RegressionResult *>::const_iterator
    RegressionSet::Begin() {
  return data_.begin();
}

boost::unordered_map<std::string, RegressionResult *>::const_iterator
    RegressionSet::End() {
  return data_.end();
}

void RegressionSet::Print() {
  boost::unordered_map<std::string, RegressionResult *>::iterator it;
  it = data_.begin();

  while (it != data_.end()) {
    RegressionResult *r = it->second;
    std::cout << r->ToString();
    it++;
  }
}

void RegressionSet::Load(std::string file_name) {
  std::fstream fin;
  fin.open(file_name.c_str(), std::ios::in | std::ios::binary);
  if (!fin.good()) {
    LOG(ERROR) << "could not open " << file_name;
    exit(1);
  }

  int size;

  model::LinearModel *model = new model::LinearModel();
  char buf[1024];

  while (!fin.eof()) {
    fin.read((char *)&size, sizeof(int));
    if (fin.eof()) {
      break;
    }
    fin.read(buf, size);

    model->ParseFromArray(buf, size);

    double *b = new double[model->p()];
    double *sb = new double[model->p()];
    for (int i=0;i<model->p();i++) {
      b[i] = model->b(i);
      sb[i] = model->sb(i);
    }

    RegressionResult *r = new RegressionResult(model->name(), model->n(),
        model->p(), model->r2(), model->f(), b, sb, model->intercept());

    //VLOG(1) << "loaded regression model " << model->name();

    data_[model->name()] = r;
  }
  fin.close();
}

void RegressionSet::Save(std::string file_name) {
  std::ofstream fout;
  fout.open(file_name.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);

  boost::unordered_map<std::string, RegressionResult *>::iterator it;
  it = data_.begin();

  while (it != data_.end()) {
    RegressionResult *r = it->second;

    model::LinearModel m;
    m.set_name(r->GetName());
    m.set_r2(r->GetR2());
    m.set_f(r->GetF());
    m.set_n(r->get_n());
    m.set_p(r->GetP());
    m.set_intercept(r->get_intercept());

    double *b = r->get_b();
    for (int i = 0; i < r->GetP(); i++) {
      m.add_b(b[i]);
    }

    double *sb = r->get_sb();
    for (int i = 0; i < r->GetP(); i++) {
      m.add_sb(sb[i]);
    }

    int size = m.ByteSize();
    fout.write((char *)&size, sizeof(int));
    m.SerializeToOstream(&fout);

    it++;
  }
  fout.close();
}
