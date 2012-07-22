// Copyright (c) 2012 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#ifndef UTIL_CALENDAR_H_
#define UTIL_CALENDAR_H_

#include <map>
#include <string>
#include <vector>

#include <boost/unordered_map.hpp>

#include "util/timestamp.h"

// Manages operations across days for a given trading calendar.
// For example you can iterate over a range of days as follows:
// Calendar calendar("calendar_def.txt");
// TimeStamp a(2007, 1, 3, 0, 0, 0, 0);
// TimeStamp b(2007, 1, 10, 0, 0, 0, 0);
// Timestamp t = a;
// while (t <= b) {
//   t = calendar.GetNextDay(t);
// }
// See: util/timestamp.h

class Calendar {
public:
  // Constructs a calendar from the given file. Assumes that the file
  // contains one entry per line, each entry is of the form yyyymmdd
  Calendar(const std::string& file_name);
  ~Calendar();

  // Returns the number of seconds in a trading day
  int DayLengthInSeconds() const;

  // Returns the trading day at the given offset from in. The offset can
  // be positive or negative.
  TimeStamp GetNext(const TimeStamp& in, int offset) const;

  TimeStamp GetNextDay(const TimeStamp& in) const;
  TimeStamp GetPrevDay(const TimeStamp& in) const;

  // Sets t to the start of the trading day
  static TimeStamp StartOfDay(const TimeStamp& t);

  // Sets t to the end of the trading day
  static TimeStamp EndOfDay(const TimeStamp& t);

  // Returns ts + seconds. Seconds can be positive or negative.
  TimeStamp AddSeconds(const TimeStamp& ts, int seconds) const;

  // Returns the difference in seconds between ts2 and ts1.
  // Requires ts1 and ts2 to be market open times
  // But no assumption about the relationship, i.e. ts1[<|==|>]ts2 all fine
  int DifferenceInSeconds(const TimeStamp& ts1, const TimeStamp& ts2) const;

  // Returns the difference in days between ts2 and ts1.
  // Requires ts1 and ts2 to be market open times
  // But no assumption about the relationship, i.e. ts1[<|==|>]ts2 all fine
  int DifferenceInDays(const TimeStamp& ts1, const TimeStamp& ts2) const;

  bool IsMarketOpen(const TimeStamp& ts) const;

  // Check whether a time stamp is within market open hours
  static bool ValidHours(const TimeStamp& ts);

private:
  // Disallow copy and assignment
  explicit Calendar(const Calendar& copy_from_me);
  void operator=(const Calendar&);

  // Map of day offset by encoded time. Used  to efficiently find a day.
  boost::unordered_map<unsigned long, int> days_;

  // All valid days as an array.
  std::vector<TimeStamp> days_vec_;

  void ToDayHourMinuteSecond(unsigned long seconds, int parts[4]) const;

  static const int kDayLengthInSeconds = 6.5 * 60 * 60;
};

#endif /* UTIL_CALENDAR_H_ */
