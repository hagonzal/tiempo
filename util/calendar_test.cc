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

#include "util/calendar.h"
#include "util/timestamp.h"
#include "util/util.h"

class CalendarTest : public ::testing::Test {
 protected:
  CalendarTest() {
  }

  virtual ~CalendarTest() {
  }

  virtual void SetUp() {
  }

  virtual void TearDown() {
  }
};

typedef CalendarTest CalendarDeathTest;

Calendar calendar("../trading_data/calendar.txt");

TEST_F(CalendarTest, test_start_end_of_day) {
  int year = 2011;
  int month = 1;
  int day = 3;
  int hour = 9;
  int min = 30;
  int sec = 16;
  int mill = 167;

  TimeStamp ts(year, month, day, hour, min, sec, mill * 1000);
  TimeStamp ts_start(year, month, day, 9, 30, 0, 0);
  TimeStamp ts_end(year, month, day, 16, 0, 0, 0);

  ASSERT_TRUE(calendar.StartOfDay(ts) == ts_start);

  ASSERT_TRUE(calendar.EndOfDay(ts) == ts_end);
}

TEST_F(CalendarDeathTest, test_next_day) {
  int year = 2011;
  int month = 1;
  int day = 3;
  int hour = 9;
  int min = 30;
  int sec = 16;
  int mill = 167;

  TimeStamp ts(year, month, day, hour, min, sec, mill * 1000);
  TimeStamp ts_next(year, month, day + 1, 9, 30, 0, 0);
  // +5 business days, i.e. +7 calendar days
  TimeStamp ts_next5(year, month, day + 7, 9, 30, 0, 0);

  // weekend cannot be found in the calendar
  TimeStamp ts_weekend(2011, 1, 8, 0, 0, 0, 0);
  ASSERT_EXIT(calendar.GetNextDay(ts_weekend), ::testing::ExitedWithCode(1),
      "day not found");

  // last day available from calendar.txt
  TimeStamp ts_lastDay(2012, 3, 9, 0, 0, 0, 0);
  ASSERT_EXIT(calendar.GetNextDay(ts_lastDay), ::testing::ExitedWithCode(1),
        "end of available calendar");

  TimeStamp ts1 = calendar.GetNextDay(ts);

  ASSERT_TRUE(ts1 == ts_next);

  TimeStamp ts2 = calendar.GetNext(ts, 5);
  ASSERT_TRUE(ts2 == ts_next5);
}

TEST_F(CalendarTest, test_market_open) {
  int year = 2011;
  int month = 1;
  int day = 3;
  int hour = 9;
  int min = 30;
  int sec = 16;
  int mill = 167;

  TimeStamp ts(year, month, day, hour, min, sec, mill * 1000);
  TimeStamp ts_before_open(year, month, day, 8, 0, 0, 0);
  TimeStamp ts_after_close(year, month, day, 17, 0, 0, 0);
  TimeStamp ts_holiday(year, 1, 1, 10, 0, 0, 0);

  ASSERT_TRUE(calendar.IsMarketOpen(ts));
  ASSERT_FALSE(calendar.IsMarketOpen(ts_before_open));
  ASSERT_FALSE(calendar.IsMarketOpen(ts_after_close));
  ASSERT_FALSE(calendar.IsMarketOpen(ts_holiday));
}

TEST_F(CalendarTest, test_difference) {
  int year = 2011;
  int month = 1;
  int day = 3;
  int hour = 9;
  int min = 30;
  int sec = 16;
  int mill = 167;

  int dDay = 5;
  int dHour = 2;
  int dMin = 23;
  int dSec = 35;

  TimeStamp ts(year, month, day, hour, min, sec, mill * 1000);
  // 5 business days = 7 calendar days
  TimeStamp ts_next5(year, month, day + 7, hour+dHour, min+dMin, sec+dSec, 0);
  TimeStamp ts_weekend(2011, 1, 8, 10, 0, 0, 0);

  int dayInSeconds = calendar.DayLengthInSeconds();
  int diffSeconds = dDay * dayInSeconds + dHour * 3600 + dMin * 60 + dSec;

  ASSERT_EQ(calendar.DifferenceInDays(ts, ts_next5), dDay);
  ASSERT_EQ(calendar.DifferenceInDays(ts, ts_next5), dDay);

  ASSERT_EQ(calendar.DifferenceInDays(ts, ts_weekend), -1);
  ASSERT_EQ(calendar.DifferenceInDays(ts_weekend, ts_next5), -1);

  ASSERT_EQ(calendar.DifferenceInSeconds(ts, ts_next5), diffSeconds);
  ASSERT_EQ(calendar.DifferenceInSeconds(ts_next5, ts), diffSeconds);

  ASSERT_EQ(calendar.DifferenceInSeconds(ts, ts_weekend), -1);
  ASSERT_EQ(calendar.DifferenceInSeconds(ts_weekend, ts_next5), -1);
}

TEST_F(CalendarDeathTest, test_difference) {
  TimeStamp ts1, ts2;
  ts1.ParseDateTime("20070103", "9:33:22.167");
  ts2.ParseDateTime("20061229","15:58:22.167");

  int diff = calendar.DifferenceInSeconds(ts1, ts2);

  ASSERT_EQ(300, diff);

  ts2.ParseDateTime("20090521", "09:33:22.167");
  diff = calendar.DifferenceInSeconds(ts1, ts2);

  ASSERT_EQ(23400 * 600, diff);
}


TEST_F(CalendarDeathTest, test_add_seconds_across_days) {
  TimeStamp ts;
  ts.ParseDateTime("20070103", "9:33:22.167");

  TimeStamp expected;
  expected.ParseDateTime("20061229","15:58:22.167");

  TimeStamp result = calendar.AddSeconds(ts, -300);
  ASSERT_EQ(expected, result);

  result = calendar.AddSeconds(result, 300);
  ASSERT_EQ(result, ts);

  result = calendar.AddSeconds(ts, -10);
  expected.ParseDateTime("20070103","9:33:12.167");
  ASSERT_EQ(expected, result);

  result = calendar.AddSeconds(ts, 300);
  expected.ParseDateTime("20070103","9:38:22.167");
  ASSERT_EQ(expected, result);

  result = calendar.AddSeconds(ts, 23400);
  expected.ParseDateTime("20070104","9:33:22.167");
  ASSERT_EQ(expected, result);

  result = calendar.AddSeconds(result, -23400);
  ASSERT_EQ(result, ts);

  result = calendar.AddSeconds(ts, 23400 * 53);
  expected.ParseDateTime("20070321","9:33:22.167");
  ASSERT_EQ(expected, result);

  result = calendar.AddSeconds(result, -23400 * 53);
  ASSERT_EQ(result, ts);

  result = calendar.AddSeconds(ts, 23400 * 600);
  expected.ParseDateTime("20090521", "09:33:22.167");
  ASSERT_EQ(expected, result);

  result = calendar.AddSeconds(result, -23400 * 600);
  ASSERT_EQ(result, ts);
}


TEST_F(CalendarDeathTest, test_add_seconds) {
  int year = 2011;
  int month = 1;
  int day = 3;
  int hour = 9;
  int min = 30;
  int sec = 16;
  int mill = 167;

  int dDay = 5;
  int dHour = 2;
  int dMin = 23;
  int dSec = 35;

  TimeStamp ts(year, month, day, hour, min, sec, mill * 1000);
  TimeStamp ts_dSec(year, month, day, hour, min, sec + dSec, mill * 1000);
  TimeStamp ts_dMin(year, month, day, hour, min + dMin, sec, mill * 1000);
  TimeStamp ts_dHour(year, month, day, hour + dHour, min, sec, mill * 1000);

  // 5 business days = 7 calendar days
  TimeStamp ts_dDay(year, month, day + 7, hour, min, sec, mill * 1000);
  TimeStamp ts_next5(year, month, day + 7, hour + dHour, min + dMin,
      sec + dSec, mill * 1000);


  int dayInSeconds = 6.5 * 60 * 60; // calendar.DayLengthInSeconds();
  int diffSeconds = dDay * dayInSeconds + dHour * 3600 + dMin * 60 + dSec;

  TimeStamp ts_weekend(2011, 1, 8, 10, 0, 0, 0);
  ASSERT_EXIT(calendar.AddSeconds(ts_weekend, dSec),
      ::testing::ExitedWithCode(1), "market closed at time");

  TimeStamp t1 = calendar.AddSeconds(ts, dSec);
  ASSERT_TRUE(t1 == ts_dSec);

  TimeStamp t2 = calendar.AddSeconds(ts, dMin * 60);
  ASSERT_TRUE(t2 == ts_dMin);

  TimeStamp t3 = calendar.AddSeconds(ts, dHour * 3600);
  ASSERT_TRUE(t3 == ts_dHour);

  TimeStamp t4 = calendar.AddSeconds(ts, dDay * dayInSeconds);
  ASSERT_TRUE(t4 == ts_dDay);

  TimeStamp t5 = calendar.AddSeconds(ts, diffSeconds);
  ASSERT_TRUE(t5 == ts_next5);
}

int main(int argc, char **argv) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  ::testing::InitGoogleTest(&argc, argv);
  google::InitGoogleLogging(argv[0]);

  return RUN_ALL_TESTS();
}
