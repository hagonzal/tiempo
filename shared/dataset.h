// Copyright (c) 2012 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#ifndef SHARED_DATASET_H_
#define SHARED_DATASET_H_

#include "util/timestamp.h"

#include <map>
#include <vector>

// Stores a collection of time series each of the Same length
// Timeseries are stored and retrieved by name:
// For example:
// dataset.GetTimeSeries("MSFT_r")
// The name of a time series is of the form prefix_suffix
// where prefix is usually a symbol name, e.g. MSFT
// and suffix is a particular time series on that symbol, e.g. _b (bid)
// an few examples of names are: MSFT_b (bid for MSFT),
// MSFT_r_ma (return weighted average for MSFT), SPY_b, SPY_bs
class DataSet {
 public:
  DataSet();
  ~DataSet();

  // Prints all time series in the dataset.
  void Print();

  // Prints all time series that match the given pattern.
  // pattern: regular expression for symbols to print, e.g. "MSFT.*"
  // valid: if not NULL, we will only print entries with valid[i] = 1.0
  void Print(const std::string& pattern, double *valid);

  // Returns the length of each time series. The lenght is constant for
  // all time series in the dataset.
  int GetDimension();

  void SetDimension(int n);

  // Sets the timestamp of the first observation of the time series. All
  // time series in the dataset share the start time.
  void SetStartTime(const TimeStamp& timestamp);
  TimeStamp GetStartTime();

  // Sets the gap in seconds between entries in each time series. All time
  // series in the dataset share the same gap.
  void SetSecondsPerUnit(int seconds_per_unit);
  int GetSecondsPerUnit();

  // Adds a time series to the dataset. Assumes that the size of row is
  // the Dimension value.
  void AddTimeSeries(const std::string& name, double *row);
  double *GetTimeSeries(const std::string& name);

  void RemoveTimeSeries(const std::string& name);

  // Returns the set of base name in the dataset.
  // For example if the dataset contains the following time series:
  // MSFT_b, MSFT_a, SPY_b, SPY_a
  // GetBaseNames returns MSFT,SPY
  void GetBaseNames(std::vector<std::string>& result);

  // Deletes all time series with the given suffix
  void DeleteSuffix(const std::string& suffix);

  // Deletes all time series with the given prefix
  void DeletePrefix(const std::string& prefix);

  // Concatenates the time series in this dataset with the time series
  // in the other dataset. Modifies the current dataset in the following way
  // time series that are present in both datasets will be concatenated.
  // time series present only in this dataset but not in other will be
  // deleted.
  // time series present only in the other dataset wil be ignored
  void Concatenate(const DataSet& other);

  // This is equivalent to the assignment operator
  void FillFrom(const DataSet& other);

  // Deletes all time series in the dataset.
  void Clear();

  // Returns a constant iterator to the first entry the data set.
  std::map<std::string, double *>::const_iterator Begin();

  // Returns a constant iterator tothe last entry in the data set.
  std::map<std::string, double *>::const_iterator End();

  // Returns the prefix of a name, e.g., "MSFT" for "MSFT_r"
  static std::string ComputePrefix(const std::string& a);

  // Returns the suffix of a name, e.g., "_r" for "MSFT_r"
  static std::string ComputeSuffix(const std::string& a);

  // Computes the or of two valid vectors. A valid vector is composed of
  // 1.0 or 0.0 entries. The result is placed in r.
  void ComputeOr(const std::string& v1, const std::string& v2, double *r);

  // Computes the and of two valid vectors. The result is placed in r.
  void ComputeAnd(const std::string& v1, const std::string& v2, double *r);

 private:
  // Disallow copy and assign.
  explicit DataSet(const DataSet& copy_from_me);
  void operator=(const DataSet&);

  // Number of entries in each time series.
  int dimension_;

  // Sorted map of time series by name.
  std::map<std::string, double *> data_;

  // Timestamp of the first entry of each time series.
  TimeStamp start_time_;

  // Gap in seconds between time series entries.
  int seconds_per_unit_;
};

#endif /* SHARED_DATASET_H_ */
