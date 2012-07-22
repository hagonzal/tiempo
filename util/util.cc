// Copyright (c) 2011 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#include "util/util.h"

#include <math.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

double Elapsed(time_t start, time_t finish) {
  return ((double) (finish - start) / (double) CLOCKS_PER_SEC);
}

char * ReadFile(const char *name, int &size) {
  // Get the file size
  struct stat filestatus;
  stat(name, &filestatus);
  size = filestatus.st_size;

  std::fstream fin;
  fin.open(name, std::ios::in | std::ios::binary);

  if (!fin.good()) {
    std::cout << "could not open file " << name;
    exit(1);
  }

  // Read the contents into a buffer
  char *buf = new char[size];
  fin.read(buf, size);
  return buf;
}

bool SameContent(const char* actual_file, const char* expected_file) {
  int size1, size2;
  char *actual = ReadFile(actual_file, size1);
  char *expected = ReadFile(expected_file, size2);

  if (size1 != size2) {
    return false;
  }

  bool ok = true;
  for (int i = 0; i < size1; i++) {
    if (expected[i] != actual[i]) {
      ok = false;
      break;
    }
  }

  delete[] actual;
  delete[] expected;

  return ok;
}

std::string Trim(const std::string& line) {
  int n = line.length();
  int start = 0;
  const char *buf = line.c_str();

  // trim left
  while (start < n && buf[start] == ' ') {
    start++;
  }

  if (start >= n) {
    return "";
  }

  // trim right
  int end = n - 1;
  while (end >= 0 && buf[end] == ' ') {
    end--;
  }

  if (end < 0) {
    return "";
  }

  return line.substr(start, end - start + 1);
}

int Tokenize(const std::string& line, char sep, std::string *out) {
  int n = line.length();
  const char* s = line.c_str();

  int pos = 0;
  int start = 0;
  for (int i = 0; i < n; i++) {
    if (s[i] == sep) {
      out[pos] = line.substr(start, i - start);

      start = i + 1;
      pos++;
    }
  }

  if (start <= n - 1) {
    out[pos++] = line.substr(start, n - start);
  }

  return pos;
}

// stringstream is slow. Processing a file with tick data (~250MB) with
// this function is 50% slower than processing it with Tokenize.
int TokenizeSlow(std::string& line, std::string *out) {
  std::stringstream stream(line);

  int i=0;
  while(getline(stream, out[i], ',')) {
    i++;
  }

  return i;
}

// Lookup table for first several powers of 10, mainly for efficiency
// Others revert to standard call to pow(double, double)
// Used to support variable precision time stamp
int ten_exp[] = {1, 10, 100, 1000, 10000, 100000, 1000000};
int IntPower(int base, int expo) {
  if (base == 10 && expo >= 0 && expo <= 6) {
    return ten_exp[expo];
  }

  // revert to standard function. Otherwise need to handle negative exponents.
  return (int) pow((double) base, (double) expo);
}

// Maps a short string with A-Z letters to a unique integer.
int pow26[] = {1, 26, 676, 17576, 456976, 11881376};
unsigned int SymbolToInt(const std::string& symbol) {
  int pow = 1;
  int val = 0;
  for (unsigned int i = 0; i < symbol.length(); i++) {
    int a = symbol.at(i) - 64;
    val += a * pow26[i];
    pow *= 26;
  }
  return val;
}

double IntToDouble(int v) {
  double d = (int) (v / 10000) +
      ((double) (v % 10000) / 10000);

  return d;
}

unsigned int DoubleToInt(double d) {
  int int_part = d;
  int frac_part = round((d - int_part) * 10000);

  return int_part * 10000 + frac_part;
}

void RemoveTrailingZeroes(char *value) {
  int i = strlen(value) - 1;
  while (i >= 0 && value[i] == '0') {
    i--;
  }

  i++;
  value[i] = 0;
  if (i > 0 && value[i - 1] == '.') {
    value[i] = '0';
    value[i+1] = 0;
  }
}
