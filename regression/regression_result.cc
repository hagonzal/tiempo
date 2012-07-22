// Copyright (c) 2012 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#include "regression/regression_result.h"

#include <string>
#include <sstream>

#include <glog/logging.h>

RegressionResult::RegressionResult(std::string name, int n, int p, double r2,
    double f, double *b, double *sb, bool intercept) {
  name_ = name;
  n_ = n;
  p_ = p;
  r2_ = r2;
  f_ = f;
  b_ = b;
  sb_ = sb;
  intercept_ = intercept;
}

RegressionResult::~RegressionResult() {
  delete[] b_;
  delete[] sb_;
}

double RegressionResult::Eval(double *x) {
  int inter = intercept_ ? 1 : 0;
  double r = intercept_ ? b_[0] : 0;
  for (int i = 0; i < p_ - inter; i++) {
    r += b_[i + inter] * x[i];
  }
  return r;
}

std::string RegressionResult::GetName() {
  return name_;
}

std::string RegressionResult::ToString() {
  std::stringstream stream;

  stream << name_ << "\n";
  stream << "n " << n_ << " p " << p_ << "\n";
  stream << "r2 " << GetR2() << " f " << GetF() << "\n";

  for (int i = 0; i < p_; i++) {
    stream << "b" << "[" << i << "] " << b_[i] << " ";
  }
  stream << "\n";

  for (int i = 0; i < p_; i++) {
    stream << "sb" << "[" << i << "] " << sb_[i] << " ";
  }
  stream << "\n";
  stream << "intercept " << intercept_ << "\n";

  return stream.str();
}
