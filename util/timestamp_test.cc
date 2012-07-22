// Copyright (c) 2012 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Ethan Liang (ethan.liang.el@gmail.com)

#include <sys/stat.h>

#include <unistd.h>
#include <iostream>
#include <fstream>

#include <glog/logging.h>
#include "gtest/gtest.h"

#include "util/util.h"
#include "util/timestamp.h"
#include "proto/tick.pb.h"

class TimeStampTest : public ::testing::Test {
 protected:
  TimeStampTest() {
  }

  virtual ~TimeStampTest() {
  }

  virtual void SetUp() {
  }

  virtual void TearDown() {
  }
};

void AssertTimeMatches(TimeStamp t, unsigned long year, unsigned long month,
    unsigned long day, unsigned long hour, unsigned long minute,
    unsigned long second, unsigned long millis) {
  ASSERT_EQ(year, t.GetYear());
  ASSERT_EQ(month, t.GetMonth());
  ASSERT_EQ(day, t.GetDay());
  ASSERT_EQ(hour, t.GetHour());
  ASSERT_EQ(minute, t.GetMinute());
  ASSERT_EQ(second, t.GetSecond());
  ASSERT_EQ(millis * 1000, t.GetMicros());
}

void AssertTimeMatches(TimeStamp t1, TimeStamp t2) {
  ASSERT_EQ(t1.GetYear(), t2.GetYear());
  ASSERT_EQ(t1.GetMonth(), t2.GetMonth());
  ASSERT_EQ(t1.GetDay(), t2.GetDay());
  ASSERT_EQ(t1.GetHour(), t2.GetHour());
  ASSERT_EQ(t1.GetMinute(), t2.GetMinute());
  ASSERT_EQ(t1.GetSecond(), t2.GetSecond());
  ASSERT_EQ(t1.GetMicros(), t2.GetMicros());
}

TEST_F(TimeStampTest, test_timestamp_constructor) {
  int year = 2011;
  int month = 1;
  int day = 3;
  int hour = 9;
  int min = 30;
  int sec = 16;
  int mill = 167;

  data::Time inputTime;
  inputTime.set_year(year);
  inputTime.set_month(month);
  inputTime.set_day(day);
  inputTime.set_hour(hour);
  inputTime.set_minute(min);
  inputTime.set_second(sec);
  inputTime.set_millis(mill);

  TimeStamp ts1(year, month, day, hour, min, sec, mill * 1000);
  AssertTimeMatches(ts1, year, month, day, hour, min, sec, mill);

  TimeStamp ts2(inputTime);
  AssertTimeMatches(ts2, year, month, day, hour, min, sec, mill);
}

TEST_F(TimeStampTest, test_string_parser) {

  std::string sdate = "20110103";
  std::string stime = "9:30:16.167";

  TimeStamp t1(2011, 1, 3, 0, 0, 0, 0);
  TimeStamp t2(2011, 1, 3, 9, 30, 16, 167 * 1000);

  TimeStamp t1s;
  t1s.ParseDate(sdate);
  AssertTimeMatches(t1, t1s);

  TimeStamp t2s;
  t2s.ParseDateTime(sdate, stime);
  AssertTimeMatches(t2, t2s);

}

TEST_F(TimeStampTest, test_set_time) {
  int year = 2011;
  int month = 1;
  int day = 3;
  int hour = 9;
  int min = 30;
  int sec = 16;
  int mill = 167;

  data::Time inputTime;
  inputTime.set_year(year);
  inputTime.set_month(month);
  inputTime.set_day(day);
  inputTime.set_hour(hour);
  inputTime.set_minute(min);
  inputTime.set_second(sec);
  inputTime.set_millis(mill);

  TimeStamp ts1;
  ts1.SetTime(inputTime);
  AssertTimeMatches(ts1, year, month,day, hour, min, sec, mill);

  TimeStamp ts2;
  ts2.SetTime(year, month, day, hour, min, sec, mill * 1000);
  AssertTimeMatches(ts2, year, month,day, hour, min, sec, mill);
}

TEST_F(TimeStampTest, test_operator) {
  int year = 2011;
  int month = 1;
  int day = 3;
  int hour = 9;
  int min = 30;
  int sec = 16;
  int mill = 167;

  TimeStamp ts1(year, month, day-1, hour, min, sec, mill * 1000);
  TimeStamp ts2(year, month, day-1, hour+1, min, sec, mill * 1000);
  TimeStamp ts3(year, month, day, hour, min, sec, mill * 1000);
  TimeStamp ts4(year, month, day, hour+1, min, sec, mill * 1000);

  TimeStamp ts0 = ts1;

  // expected relationship ts0 = ts1 < ts2 < ts3 < ts4

  ASSERT_TRUE(ts0 == ts1);
  ASSERT_TRUE(ts1 < ts2);
  ASSERT_FALSE(ts2 > ts3);
  ASSERT_FALSE(ts3 == ts4);
  ASSERT_TRUE(ts3 < ts4);
}

TEST_F(TimeStampTest, test_clear_time) {
  TimeStamp t1(2011, 1, 3, 9, 30, 16, 167 * 1000);
  TimeStamp t2(2011, 1, 3, 0, 0, 0, 0);

  t1.ClearTime();

  AssertTimeMatches(t1, t2);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  google::InitGoogleLogging(argv[0]);

  return RUN_ALL_TESTS();
}
