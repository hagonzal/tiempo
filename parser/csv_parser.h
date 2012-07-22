// Copyright (c) 2012 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#ifndef PARSER_CSVPARSER_H_
#define PARSER_CSVPARSER_H_

#include <fstream>

class DataSet;

// Parses a csv file.
// Example use:
// CsvParser parser(file_name);
// std::string parts[10];
// while (parser.NextLine(parts) != 0) {
//   // process parts
// }
// Can also be used to parse a single line
// CsvParser::Tokenize("a,b,c", ",", out);
class CsvParser {
 public:
  CsvParser(const char *file_name);
  ~CsvParser();

  // Parses the next line placing each component in out. It is the
  // reponsibility of the caller to make sure that out is large enough
  // to store field_count_ entries.
  int NextLine(std::string *out);

  // Returns the number of fields in the csv file.
  int GetFieldCount();

 private:
  // Disallow copy and assign
  explicit CsvParser(const CsvParser& copy_from_me);
  void operator=(const CsvParser&);

  std::fstream fin_;
  int field_count_;
};

#endif /* PARSER_CSVPARSER_H_ */
