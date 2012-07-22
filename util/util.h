// Copyright (c) 2011 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#ifndef UTIL_TEST_UTIL_H_
#define UTIL_TEST_UTIL_H_

#include <math.h>
#include <sys/stat.h>

#include <string>

// Returns the elapsed time between start and end in seconds.
double Elapsed(time_t start, time_t finish);

// Reads the given file and returns a buffer with the contents.
char * ReadFile(const char *name, int &size);

// Returns true if the content of the two given files is thesame, false
// otherwise.
bool SameContent(const char* actual_file, const char* expected_file);

// Returns true if the two doubles are the Same at 6 decimal digits
inline bool SameDouble(double d1, double d2) {
  double d = fabs(d1 - d2);

  if (d <= 0.000001) {
    return true;
  }
  return false;
}

std::string Trim(const std::string& line);

// Divides the input line into parts separated by sep. Places the resulting
// parts in out. Returns the number of parts. It is the responsibility of the
// caller to make out large enough to hold the parts.
int Tokenize(const std::string& line, char sep, std::string *out);

// Returns the number of tokens in the input string separated with sep.
int CountTokens(const std::string& line, char sep);

// Same as Tokenize but slower. Here only for illustration purposes.
int TokenizeSlow(std::string& line, std::string *out);

// calculate integer power, @base raised by @expo
int IntPower(int base, int expo);

// Converts a symbol to a unique integer
unsigned int SymbolToInt(const std::string& symbol);

// Returns a fixed precision integer. int_part * 10000 + frac_part.
unsigned int DoubleToInt(double d);

// Converts an fixed precision integer to a double. This is the inverse
// of DoubleToInt.
double IntToDouble(int v);

// Removes trailing zeroes. Examples
// 12.232000 -> 12.232
// 12.0000 -> 12.0
void RemoveTrailingZeroes(char *value);

#endif /* UTIL_TEST_UTIL_H_ */
