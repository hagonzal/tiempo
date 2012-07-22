// Copyright (c) 2012 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#ifndef SIMULATOR_REGRESSIONSET_H_
#define SIMULATOR_REGRESSIONSET_H_

#include <list>
#include <string>

#include <boost/unordered_map.hpp>

class RegressionResult;

// Contains a set of regression. Each regression is identified by a name.
class RegressionSet {
public:
  RegressionSet();
  ~RegressionSet();

  // Loads the regression set from disk
  void Load(std::string file_name);

  // Saves the regression set to disk
  void Save(std::string file_name);

  // Adds a new regression to the set
  void Add(RegressionResult *r);

  // Retrieves a regression by name
  RegressionResult *Get(std::string name);

  // Prints all regression models to stdout
  void Print();

  // Removes all regression models from the set.
  void Clear();

  boost::unordered_map<std::string, RegressionResult *>::const_iterator
    Begin();

  boost::unordered_map<std::string, RegressionResult *>::const_iterator
    End();


private:
  boost::unordered_map<std::string, RegressionResult *> data_;

  RegressionResult *GetInternal(std::string name);
};

#endif /* SIMULATOR_REGRESSIONSET_H_ */
