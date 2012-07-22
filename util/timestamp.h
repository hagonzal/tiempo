// Copyright (c) 2012 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#ifndef UTIL_TIMESTAMP_H_
#define UTIL_TIMESTAMP_H_

#include <string>

#include "proto/tick.pb.h"

// Stores time as year, month, day, hour, second, micros.
// All the components of the date are encoded into a single
// unsigned long. Provides ==, <, and > operators.
//
// See Calendar (parser/calendar.h) for more operations on timestamps
// across days.
class TimeStamp {
public:
  TimeStamp();
  TimeStamp(const data::Time& time);

  TimeStamp(int year, int month, int day, int hour, int minute, int second,
      int micros);

  TimeStamp(unsigned long encoded_time);
  virtual ~TimeStamp();

  bool operator<(const TimeStamp& other) const;
  bool operator>(const TimeStamp& other) const;
  bool operator==(const TimeStamp& other) const;

  // Clears the hour, minute, seconds, and micros components.
  void ClearTime();

  void SetTime(const data::Time& time);
  void SetTime(int year, int month, int day, int hour, int minute, int second,
      int micros);

  unsigned long GetEncodedTime();

  void GetTime(data::Time& time);

  void ParseDateTime(const std::string& sdate, const std::string& stime);
  void ParseDate(const std::string& sdate);

  unsigned long GetYear() const;
  unsigned long GetMonth() const;
  unsigned long GetDay() const;
  unsigned long GetHour() const;
  unsigned long GetMinute() const;
  unsigned long GetSecond() const;
  unsigned long GetMicros() const;

  std::string ToString(int level) const;
  std::string ToString() const;
private:
  unsigned long encoded_time_;

  static const int kTimeUnitBits[];
  static const int kTimeUnitOffsets[];

  unsigned long Encode(int year, int month, int day, int hour, int minute,
      int second, int micros);
};

#endif /* UTIL_TIMESTAMP_H_ */
