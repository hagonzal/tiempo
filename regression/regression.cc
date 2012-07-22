// Copyright (c) 2012 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#include "regression/regression.h"

#include <armadillo>
#include <glog/logging.h>

#include "proto/model.pb.h"
#include "regression/regression_result.h"
#include "shared/dataset.h"

Regression::Regression() {
}

Regression::~Regression() {
}

void FillXNoIntercept(DataSet& dataset, std::vector<std::string>&
    x_names, double *v, int p, int valid_n, int orig_n, arma::mat& X) {

  for (int col = 0; col < p; col++) {
    double *x_ts = dataset.GetTimeSeries(x_names[col]);
    int row_count = 0;
    for (int row = 0; row < orig_n; row++) {
      if (v == NULL || v[row] == 1.0) {
        X(row_count, col) = x_ts[row];
        row_count++;
      }
    }
  }
}

void FillXIntercept(DataSet& dataset, std::vector<std::string>& x_names,
    double *v, int p, int valid_n, int orig_n, arma::mat& X) {

  for (int i = 0; i < valid_n; i++) {
    X(i, 0) = 1.0;
  }

  for (int col = 0; col < p - 1; col++) {
    double *x_ts = dataset.GetTimeSeries(x_names[col]);
    int row_count = 0;
    for (int row = 0; row < orig_n; row++) {
      if (v == NULL || v[row] == 1.0) {
        X(row_count, col + 1) = x_ts[row];
        row_count++;
      }
    }
  }
}

void FillY(DataSet& dataset, std::string y_name, double *v, int orig_n,
    arma::mat& Y) {
  double *y_ts = dataset.GetTimeSeries(y_name);

  int count = 0;
  for (int i = 0; i < orig_n; i++) {
    if (v == NULL || v[i]) {
      Y(count++, 0) = y_ts[i];
    }
  }
}

int CountValid(double *v, int n) {
  int valid_n = 0;
  for (int i = 0; i < n; i++) {
    int a = v[i];
    if (a == 1) {
      valid_n++;
    }
  }
  return valid_n;
}

RegressionResult *Regression::Compute(DataSet& dataset, std::string y_name,
    std::vector<std::string>& x_names, double *v, bool intercept) {

  int inter = intercept ? 1 : 0;

  int orig_n = dataset.GetDimension(); // Number of examples
  int p = x_names.size() + inter;
  int valid_n = orig_n;

  if (v != NULL) {
    valid_n = CountValid(v, orig_n);
  }

  arma::mat Y(valid_n, 1);
  arma::mat X(valid_n, p);

  VLOG(1) << "valid n " << valid_n << "  orig n " << orig_n << " p " << p;

  FillY(dataset, y_name, v, orig_n, Y);

  if (intercept) {
    FillXIntercept(dataset, x_names, v, p, valid_n, orig_n, X);
  } else {
    FillXNoIntercept(dataset, x_names, v, p, valid_n, orig_n, X);
  }

  arma::mat X_t = X.t();
  arma::mat XXi = (X_t * X).i();
  arma::mat B = XXi * X_t * Y;

  arma::mat Yh = X * B;

  double Ym = arma::as_scalar(arma::mean(Y));

  double SSM, SSE, SST;
  double MSM, MSE, MST;

  if (intercept) {
    SSM = arma::as_scalar((Yh - Ym).t() * (Yh - Ym));
    SSE = arma::as_scalar((Y - Yh).t() * (Y - Yh));
    SST = arma::as_scalar((Y - Ym).t() * (Y - Ym));

    MSM = SSM / (p - 1);
    MSE = SSE / (valid_n - p);
    MST = SST / (valid_n - 1);
  } else {
    SSM = arma::as_scalar(Yh.t() * Yh);
    SSE = arma::as_scalar((Y - Yh).t() * (Y - Yh));
    SST = arma::as_scalar(Y.t() * Y);

    MSM = SSM / p;
    MSE = SSE / (valid_n - p);
    MST = SST / (valid_n - 1);
  }

  double r2 = SSM / SST;
  double f = MSM / MSE;

  arma::mat sig_b = arma::sqrt(MSE * arma::diagvec(XXi));

  double *b = new double[p];
  double *sb = new double[p];

  for (int i = 0; i < p; i++) {
    b[i] = B(i, 0);
    sb[i] = sig_b(i, 0);
  }

  std::stringstream stream;
  stream << y_name;
  for (int j = 0; j < p - inter; j++) {
    stream << ":" << x_names[j];
  }
  std::string mname = stream.str();

  RegressionResult *r = new  RegressionResult(mname, valid_n, p, r2, f, b, sb,
      intercept);

  return r;
}
