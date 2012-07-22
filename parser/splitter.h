// Copyright (c) 2012 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#ifndef PARSER_SPLITTER_H_
#define PARSER_SPLITTER_H_

#include <string>

// Splits a file into multiple files. The split done along the values on
// a column, with one file per distinct value. Assumes that the file is
// sorted on the split value column.
class Splitter {
public:
  Splitter();
  ~Splitter();

  // file_name: file to split
  // split_field: field offset to use for the split
  // base_name: base name for each file the full name will be
  // base_name_splitvalue
  static void Split(const std::string& file_name, int split_field,
      const std::string& base_name);
private:
  // Disallow copy and assign
  explicit Splitter(const Splitter& copy_from_me);
  void operator=(const Splitter&);
};

#endif /* PARSER_SPLITTER_H_ */
