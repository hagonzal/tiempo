// Copyright (c) 2012 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#ifndef SHARED_DATAOPERATOR_H_
#define SHARED_DATAOPERATOR_H_

class DataSet;

// Base class for operators that work on datasets. An operator can transform
// existing entries or compute new entries in a dataset.
class DataOperator {
public:
  DataOperator();
  virtual ~DataOperator();

  virtual void Compute() = 0;

private:
  // Disallow copy and assign
  explicit DataOperator(const DataOperator& copy_from_me);
  void operator=(const DataOperator&);
};

#endif /* SHARED_DATAOPERATOR_H_ */
