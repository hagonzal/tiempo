// Copyright (c) 2012 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)
#include <iostream>

#include "util/calendar.h"

#include <glog/logging.h>

#include "parser/csv_parser.h"

const int Calendar::kDayLengthInSeconds;

Calendar::Calendar(const std::string& file_name) {
  VLOG(1) << "loading calendar from " << file_name;

  CsvParser parser(file_name.c_str());
  std::string parts[10];
  int count = 0;
  int n;
  while ((n = parser.NextLine(parts)) != 0) {
    TimeStamp ts;
    ts.ParseDate(parts[0]);

    days_[ts.GetEncodedTime()] = count;
    days_vec_.push_back(ts);

    count++;
  }
}

Calendar::~Calendar() {
}

int Calendar::DayLengthInSeconds() const {
  return kDayLengthInSeconds;
}

TimeStamp Calendar::StartOfDay(const TimeStamp& t) {
  TimeStamp result;
  result.SetTime(t.GetYear(), t.GetMonth(), t.GetDay(), 9, 30, 0, 0);
  return result;
}

TimeStamp Calendar::EndOfDay(const TimeStamp& t) {
  TimeStamp result;
  result.SetTime(t.GetYear(), t.GetMonth(), t.GetDay(), 16, 0, 0, 0);
  return result;
}

TimeStamp Calendar::GetNextDay(const TimeStamp& in) const {
  return GetNext(in, 1);
}

TimeStamp Calendar::GetPrevDay(const TimeStamp& in) const {
  return GetNext(in, -1);
}

TimeStamp Calendar::GetNext(const TimeStamp& in, int offset) const {
  TimeStamp start_ts(in);
  start_ts.ClearTime();

  boost::unordered_map<unsigned long, int>::const_iterator it =
      days_.find(start_ts.GetEncodedTime());
  if (it == days_.end()) {
    LOG(ERROR) << "day not found " << start_ts.ToString();
    exit(1);
  }

  int day_offset = it->second;
  int pos = day_offset + offset;

  if (pos < 0 || pos >= days_vec_.size()) {
    LOG(ERROR) << "end of available calendar " << pos;
    exit(1);
  }

  TimeStamp result = days_vec_[pos];
  return StartOfDay(result);
}

bool Calendar::IsMarketOpen(const TimeStamp &ts) const {
  TimeStamp t = ts;
  t.ClearTime();

  boost::unordered_map<unsigned long, int>::const_iterator it =
      days_.find(t.GetEncodedTime());

  if (it == days_.end()) {
    return false;
  }

  return ValidHours(ts);
}

bool Calendar::ValidHours(const TimeStamp &ts) {
  TimeStamp t = StartOfDay(ts);
  if(ts < t) {
    return false;
  }

  t = EndOfDay(ts);
  if(ts > t) {
    return false;
  }

  return true;
}

int Calendar::DifferenceInSeconds(const TimeStamp& ts1,
    const TimeStamp& ts2) const {
  // Difference in days between the two base days
  int days = DifferenceInDays(ts1, ts2);

  // Seconds since start of day 1
  unsigned long v1 = ts1.GetHour() * 3600 + ts1.GetMinute() * 60 +
      ts1.GetSecond();

  // Seconds since start of day 2
  unsigned long v2 = ts2.GetHour() * 3600 + ts2.GetMinute() * 60 +
      ts2.GetSecond();

  bool inverted = ts1 > ts2;

  int diff = days * DayLengthInSeconds() + (inverted ? -1 : 1) * (-v1 + v2);
  return diff;
}

int Calendar::DifferenceInDays(const TimeStamp& ts1,
    const TimeStamp& ts2) const {

  TimeStamp a = ts1;
  TimeStamp b = ts2;

  a.ClearTime();
  b.ClearTime();

  boost::unordered_map<unsigned long, int>::const_iterator it1 =
      days_.find(a.GetEncodedTime());
  boost::unordered_map<unsigned long, int>::const_iterator it2 =
      days_.find(b.GetEncodedTime());

  if (it1 == days_.end() || it2 == days_.end() ||
      !ValidHours(ts1) || !ValidHours(ts2)) {
    LOG(ERROR) << "invalid range " << ts1.ToString() << " " << ts2.ToString();
    exit(1);
  }

  int day1 = it1->second;
  int day2 = it2->second;

  return day1 > day2 ? day1 - day2 : day2 - day1;
}

TimeStamp Calendar::AddSeconds(const TimeStamp& ts, int seconds) const {
  // this method is not well defined if the market is closed at ts

  TimeStamp start = ts;
  start.ClearTime();

  boost::unordered_map<unsigned long, int>::const_iterator it =
      days_.find(start.GetEncodedTime());

  if(it == days_.end() || !ValidHours(ts)) {
      LOG(ERROR) << "market closed at time " << ts.ToString();
      exit(1);
  }

  int day_offset = it->second;

  int trading_seconds = day_offset * DayLengthInSeconds() + ts.GetHour() * 3600
      + ts.GetMinute() * 60 + ts.GetSecond() + seconds - 9.5 * 3600;

  int parts[4];
  ToDayHourMinuteSecond(trading_seconds, parts);

  // Guaranteed not to overflow the day
  parts[1] += 9;
  parts[2] += 30;
  if (parts[2] > 60) {
    parts[2] = parts[2] - 60;
    parts[1]++;
  }

  TimeStamp result = days_vec_[parts[0]];
  result.SetTime(result.GetYear(), result.GetMonth(), result.GetDay(),
      parts[1], parts[2], parts[3], ts.GetMicros());

  //VLOG(1) << "add seconds " << ts.ToString() << " " << seconds << " result "
  //    << result.ToString();

  return result;
}

void Calendar::ToDayHourMinuteSecond(unsigned long seconds, int parts[4]) const {
  parts[0] = seconds / DayLengthInSeconds();
  seconds %= DayLengthInSeconds();
  parts[1] = seconds / 3600;
  seconds %= 3600;
  parts[2] = seconds / 60;
  seconds %= 60;
  parts[3] = seconds;
}
