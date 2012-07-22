// Copyright (c) 2011 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#ifndef SHARED_DATABASE_H_
#define SHARED_DATABASE_H_

#include <string>

class TimeStamp;
class DataSet;

// Loads data files from disk and puts them into a DataSet.
class DataBase {
 public:
  // Creates a database that will Load data from data_dir
  DataBase(std::string data_dir);
  ~DataBase();

  // Reads one day of data. Assumes that the file name has the format
  // yyyymmdd.quote.seconds_per_unit. Adds timeseries SYMBOL_(b,bs,a,as)
  // to the dataset.
  void ReadDay(TimeStamp& day, int seconds_per_unit,
      std::string pattern, DataSet& dataset);

 private:
  // Disallow copy and assign
  explicit DataBase(const DataBase&);
  void operator=(const DataBase&);

  // Directory with the data files
  std::string data_dir_;

  std::string computeFileName(TimeStamp& day, int seconds_per_unit);
};

#endif /* SHARED_DATABASE_H_ */
