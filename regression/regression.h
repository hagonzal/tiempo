// Copyright (c) 2012 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#ifndef REGRESSION_REGRESSION_H_
#define REGRESSION_REGRESSION_H_

#include <string>
#include <vector>

class DataSet;
class RegressionResult;

// Computes a linear regression model.
class Regression {
 public:
  Regression();
  ~Regression();

  // Computes and saves linear regression models for the dataset.
  // The model to compute is defined as
  // y_name ~ x_names
  // for example
  // y_name = "MSFT_fr5" and x_names = "MSFT_r_ma,SPY_r_ma"
  // computes the model MSFT_fr5 ~ MSFT_r_ma, SPY_r_ma
  // in order to compute the model the time series
  // MSFT_fr5, MSFT_r_ma, and SPY_r_ma need to be present in the dataset
  // v: v[i] is 1 if entry i is valid, 0 otherwise
  // intersect: true if you want to build a model with intersect, false
  // otherwise.
  static RegressionResult *Compute(DataSet &dataset, std::string y_name,
      std::vector<std::string>& x_names, double *v, bool intersect);

 private:
  // Disallow copy and assign
  explicit Regression(const Regression& copy_from_me);
  void operator=(const Regression&);
};

#endif /* REGRESSION_REGRESSION_H_ */
