// Copyright (c) 2011 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#include "parser/group.h"

#include <stdio.h>

#include <iostream>

#include "proto/tick.pb.h"

Group::Group(data::Tick *tick, int secondsPerUnit) {
  seconds_per_unit_ = secondsPerUnit;
  units_ = ComputeUnitCount(seconds_per_unit_);

  ask_ = new double[units_];
  bid_ = new double[units_];
  ask_size_ = new int[units_];
  bid_size_ = new int[units_];

  group_.set_symbol(tick->symbol());

  data::Time * gtime = group_.mutable_time();
  data::Time tick_time = tick->time();
  gtime->set_year(tick_time.year());
  gtime->set_month(tick_time.month());
  gtime->set_day(tick_time.day());

  for (int i = 0; i < units_; i++) {
    ask_[i] = -1;
    bid_[i] = -1;
    ask_size_[i] = -1;
    bid_size_[i] = -1;
  }
}

Group::~Group() {
  delete[] ask_;
  delete[] bid_;
  delete[] ask_size_;
  delete[] bid_size_;
}

void Group::AddTick(const data::Tick *tick) {
  int m = ComputeCurrentUnit(seconds_per_unit_, &tick->time());

  // Keep track of the last tick
  ask_[m] = tick->value().ask();
  bid_[m] = tick->value().bid();
  ask_size_[m] = tick->value().ask_size();
  bid_size_[m] = tick->value().bid_size();
}

data::Group& Group::Proto() {
  data::TickValue *prev = NULL;

  group_.clear_values();

  for (int i = 0; i < units_; i++) {
    if (ask_[i] == -1 && prev != NULL) {
      data::TickValue* value = group_.add_values();
      value->CopyFrom(*prev);
      continue;
    }

    data::TickValue *value = group_.add_values();
    value->set_ask(ask_[i]);
    value->set_bid(bid_[i]);
    value->set_ask_size(ask_size_[i]);
    value->set_bid_size(bid_size_[i]);
    prev = value;
  }

  return group_;
}

int Group::ComputeUnitCount(int seconds_per_unit) {
  // Day = 6.5 * 60 * 60 = 23400 seconds
  return 23400 / seconds_per_unit;
}

int Group::ComputeCurrentUnit(int seconds_per_unit, const data::Time* time) {
  // TODO(hagonzal): Generalize to different trading calendars

  int sec = (time->hour() - 9) * 3600 + time->minute() * 60 + time->second();
  sec -= 30 * 60;

  int result = sec / seconds_per_unit;

  if (result < 0) {
    return 0;
  }

  if (result >= units_) {
    return units_ - 1;
  }

  return result;
}
