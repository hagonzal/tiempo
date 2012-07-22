// Copyright (c) 2012 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#include "util/timestamp.h"

#include <stdio.h>

#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>

#include <glog/logging.h>

#include "util/bit_utils.h"
#include "util/util.h"

const int TimeStamp::kTimeUnitBits[] = {12, 4, 5, 5, 6, 6, 20};
const int TimeStamp::kTimeUnitOffsets[] = {46, 42, 37, 32, 26, 20, 0};

TimeStamp::TimeStamp() {
  encoded_time_ = 0;
}

TimeStamp::TimeStamp(int year, int month, int day, int hour, int minute,
    int second, int micros) {
  encoded_time_ = Encode(year, month, day, hour, minute, second, micros);
}

TimeStamp::TimeStamp(const data::Time& time) {
  encoded_time_ = Encode(time.year(), time.month(), time.day(), time.hour(),
      time.minute(), time.second(), time.millis() * 1000);
}

TimeStamp::TimeStamp(unsigned long encoded_time) {
  encoded_time_ = encoded_time;
}

TimeStamp::~TimeStamp() {
}

unsigned long TimeStamp::GetEncodedTime() {
  return encoded_time_;
}

void TimeStamp::GetTime(data::Time& time) {
  time.set_year(GetYear());
  time.set_month(GetMonth());
  time.set_day(GetDay());
  time.set_hour(GetHour());
  time.set_minute(GetMinute());
  time.set_second(GetSecond());
  time.set_millis(GetMicros() / 1000.0);
}

void TimeStamp::ParseDate(const std::string& sdate) {
  // date is of the form: YYYYMMDD
  int year = atoi(sdate.substr(0, 4).c_str());
  int month = atoi(sdate.substr(4, 2).c_str());
  int day = atoi(sdate.substr(6, 2).c_str());
  int hour = 0;
  int minute = 0;
  int second = 0;
  int micros = 0;

  encoded_time_ = Encode(year, month, day, hour, minute, second, micros);
}

void TimeStamp::ParseDateTime(const std::string& sdate,
    const std::string& stime) {
  ParseDate(sdate);

  std::string parts[10];
  std::string seconds[10];

  int n = Tokenize(stime, ':', parts);

  int hour = 0;
  int minute = 0;
  int second = 0;
  int micros= 0;

  if(n != 3) {
    LOG(ERROR) << "invalid timestamp " << sdate << " " << stime;
    return;
  }

  hour = atoi(parts[0].c_str());
  minute = atoi(parts[1].c_str());

  // now we parse seconds and sub-seconds
  int m = Tokenize(parts[2], '.', seconds);
  if(m > 2) {
    LOG(ERROR) << "invalid timestamp " << sdate << " " << stime;
    return;
  }

  second = atoi(seconds[0].c_str());

  // only enter this block if there's indeed sub-second
  if(m == 2) {
    // need to handle variable precision of sub-seconds
    int digits = seconds[1].size();
    std::string microStr = (digits > 6) ? seconds[1].substr(0,6) : seconds[1];
    int shift = (digits > 6) ? 0 : (6 - digits);
    micros = atoi(microStr.c_str()) * IntPower(10, shift);
  }

  encoded_time_ = Encode(GetYear(), GetMonth(), GetDay(),
      hour, minute, second, micros);
}

void TimeStamp::SetTime(const data::Time& time) {
  encoded_time_ = Encode(time.year(), time.month(), time.day(), time.hour(),
        time.minute(), time.second(), time.millis() * 1000);
}

void TimeStamp::SetTime(int year, int month, int day, int hour,
    int minute, int second, int micros) {
  encoded_time_ = Encode(year, month, day, hour, minute, second, micros);
}

unsigned long TimeStamp::Encode(int year, int month, int day, int hour,
    int minute, int second, int micros) {
  // year: 12 bits, low_bit: 46
  // month: 4 bits, low_bit: 42
  // day: 5 bits, low_bit: 37
  // hour: 5 bits, low_bit: 32
  // minute: 6 bits, low_bit: 26
  // second: 6 bits, low_bit: 20
  // micro: 20 bits, low_bit 0

  return BitUtils::SetBits(year, kTimeUnitOffsets[0], kTimeUnitBits[0]) |
      BitUtils::SetBits(month, kTimeUnitOffsets[1], kTimeUnitBits[1]) |
      BitUtils::SetBits(day, kTimeUnitOffsets[2], kTimeUnitBits[2]) |
      BitUtils::SetBits(hour, kTimeUnitOffsets[3], kTimeUnitBits[3]) |
      BitUtils::SetBits(minute, kTimeUnitOffsets[4], kTimeUnitBits[4]) |
      BitUtils::SetBits(second, kTimeUnitOffsets[5], kTimeUnitBits[5]) |
      BitUtils::SetBits(micros, kTimeUnitOffsets[6], kTimeUnitBits[6]);
}

bool TimeStamp::operator<(const TimeStamp& other) const {
  return encoded_time_ < other.encoded_time_;
}

bool TimeStamp::operator>(const TimeStamp& other) const {
  return encoded_time_ > other.encoded_time_;
}

bool TimeStamp::operator==(const TimeStamp& other) const {
  return encoded_time_ == other.encoded_time_;
}

unsigned long TimeStamp::GetYear() const {
  return BitUtils::GetBits(encoded_time_, kTimeUnitOffsets[0],
      kTimeUnitBits[0]);
}

unsigned long TimeStamp::GetMonth() const {
  return BitUtils::GetBits(encoded_time_, kTimeUnitOffsets[1],
      kTimeUnitBits[1]);
}

unsigned long TimeStamp::GetDay() const {
  return BitUtils::GetBits(encoded_time_, kTimeUnitOffsets[2],
      kTimeUnitBits[2]);
}

unsigned long TimeStamp::GetHour() const {
  return BitUtils::GetBits(encoded_time_, kTimeUnitOffsets[3],
      kTimeUnitBits[3]);
}

unsigned long TimeStamp::GetMinute() const {
  return BitUtils::GetBits(encoded_time_, kTimeUnitOffsets[4],
      kTimeUnitBits[4]);
}

unsigned long TimeStamp::GetSecond() const {
  return BitUtils::GetBits(encoded_time_, kTimeUnitOffsets[5],
      kTimeUnitBits[5]);
}

unsigned long TimeStamp::GetMicros() const {
  return BitUtils::GetBits(encoded_time_, kTimeUnitOffsets[6],
      kTimeUnitBits[6]);
}

void TimeStamp::ClearTime() {
  encoded_time_ = Encode(GetYear(), GetMonth(), GetDay(), 0, 0, 0, 0);
}

std::string TimeStamp::ToString(int level) const {
  // 0 year, 1 month, 2 day, 3 hour, 4 minute, 5 second, 6 micro
  std::stringstream stream;

  int v[7];
  v[0] = GetYear();
  v[1] = GetMonth();
  v[2] = GetDay();
  v[3] = GetHour();
  v[4] = GetMinute();
  v[5] = GetSecond();
  v[6] = GetMicros();

  int w[] = {4, 2, 2, 2, 2, 2, 6};

  for (int i = 0; i <= level; i++) {
    stream << std::setw(w[i]) << std::setfill('0');
    stream << v[i];
  }

  return stream.str();
}

std::string TimeStamp::ToString() const {
  char seconds[255];
  snprintf(seconds, 255, "%02ld.%ld", GetSecond(), GetMicros());
  RemoveTrailingZeroes(seconds);

  char out[1024];
  snprintf(out, 1024, "%04ld%02ld%02ld,%02ld:%02ld:%s", GetYear(), GetMonth(),
      GetDay(), GetHour(), GetMinute(), seconds);

  return std::string(out);
}
