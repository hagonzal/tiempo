// Copyright (c) 2012 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#ifndef PARSER_CSVPARSERLOW_H_
#define PARSER_CSVPARSERLOW_H_

#include <stdio.h>

class DataSet;

// Low level csv parser
class CsvParserLow {
 public:
  CsvParserLow(const char *file_name);
  ~CsvParserLow();

  // Disallow copy constructor
  explicit CsvParserLow(const CsvParserLow& copy_from_me);

  // Returns an array of strings of length field_count_. It is the
  // responsibility of the caller to deallocate the array;
  char** NextLine();

  int GetFieldCount();

 private:
  FILE *file_;
  int field_count_;
};

#endif /* PARSER_CSVPARSERLOW_H_ */
