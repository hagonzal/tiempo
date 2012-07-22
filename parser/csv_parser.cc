// Copyright (c) 2012 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#include "parser/csv_parser.h"

#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <glog/logging.h>

#include "util/util.h"

CsvParser::CsvParser(const char *file_name) {
  fin_.open(file_name, std::ios::in);

  if (!fin_.good()) {
    LOG(ERROR) << "could not find " << file_name;
    exit(1);
  }
  field_count_ = -1;
}

CsvParser::~CsvParser() {
  fin_.close();
}

int CsvParser::GetFieldCount() {
  return field_count_;
}

int CsvParser::NextLine(std::string *parts) {
  std::string line;
  if (fin_.good()) {
    getline(fin_, line);
  }

  if (line.empty()) {
    return 0;
  }

  int n = Tokenize(line, ',', parts);

  if (field_count_ == -1) {
    field_count_ = n;
  }

  if (field_count_ != n) {
    LOG(ERROR) << "field_count_ " << field_count_ << " n " << n << "\n";
  }

  return n;
}
