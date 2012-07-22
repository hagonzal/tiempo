// Copyright (c) 2012 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#ifndef REGRESSION_REGRESSIONRESULT_H_
#define REGRESSION_REGRESSIONRESULT_H_

#include <string>

// Stores the result of a regression model.
// including Beta, r2, f, number of independent variables, etc.
// Can be used to evaluate the regression function on a new point.
class RegressionResult {
public:
  RegressionResult(std::string name, int n, int p, double r2, double f,
      double *b, double *sb, bool intercept);

  ~RegressionResult();

  // Returns with intercept_ == true
  // b_[0] + b_[1]*x[0] + ... + b_[p-1]*x[p-2]
  // with intercept_ == false
  // b_[0] * x[0] + ... + b_[p-1] * x[p-1]
  double Eval(double *x);

  std::string ToString();

  // Returns the name of the regression
  // The name is of the form
  // dep_var:ind_var_1:ind_var_2:...:ind_var_n
  std::string GetName();

  // Returns the r2 of the regression
  inline double GetR2() {
    return r2_;
  }

  // Returns the F value of the regression
  inline double GetF() {
    return f_;
  }

  // Returns the number of independent variables
  inline double GetP() {
    return p_;
  }

  // Returns the number of observations used to build the regression.
  inline double get_n() {
    return n_;
  }

  // Returns the coefficients (B)
  inline double * get_b() {
    return b_;
  }

  // Returns the standard deviation of the coefficients
  inline double * get_sb() {
    return sb_;
  }

  // Returns true if the regression has an intercept term
  inline bool get_intercept() {
    return intercept_;
  }

private:
  // Disallow copy and assign
  explicit RegressionResult(const RegressionResult&);
  void operator=(const RegressionResult&);

  std::string name_;

  int p_;
  int n_;

  double r2_;
  double f_;

  double *b_;
  double *sb_;

  bool intercept_;
};

#endif /* REGRESSION_REGRESSIONRESULT_H_ */
