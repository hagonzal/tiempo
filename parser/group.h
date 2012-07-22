// Copyright (c) 2011 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)
 
#ifndef PARSER_GROUP_H_
#define PARSER_GROUP_H_

#include "proto/tick.pb.h"

// Stores a time series for a symbol for a day.
class Group {
 public:
  Group(data::Tick* tick, int seconds_per_unit);
  virtual ~Group();

  // Adds an entry to the time series. Keeps only the last entry for each
  // group. For example, when aggregating at the minute level, it will keep
  // the last tick for each minute.
  // tick: raw tick data at the millisecond level.
  void AddTick(const data::Tick *tick);

  // Returns a protocol buffer representation of the group
  data::Group& Proto();

 private:
  // Returns the number of time units for the given aggregation level.
  int ComputeUnitCount(int seconds_per_unit);

  // Computes the time unit index for the given time.
  int ComputeCurrentUnit(int seconds_per_unit, const data::Time *time);

  // Number of elements of the time series
  int units_;

  // Level of aggregation
  int seconds_per_unit_;

  // Arrays with the elements of the time series. One entry per time unit.
  double *ask_;
  double *bid_;
  int *ask_size_;
  int *bid_size_;

  // Protocol buffer representation of the time series.
  data::Group group_;
};

#endif /* PARSER_GROUP_H_ */
