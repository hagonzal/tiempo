// Copyright (c) 2012 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#include "parser/splitter.h"

#include <stdlib.h>
#include <iostream>
#include <sstream>

#include <glog/logging.h>

#include "parser/csv_parser.h"
#include "util/util.h"

Splitter::Splitter() {
}

Splitter::~Splitter() {
}

void Splitter::Split(const std::string& file_name, int split_field,
    const std::string& base_name) {
  std::fstream fin;
  fin.open(file_name.c_str());
  if (!fin.good()) {
    LOG(ERROR) << "could not find " << file_name;
    exit(1);
  }

  std::fstream fout;


  std::string parts[20];
  std::string line;

  std::string prev_value = "";

  int count = 0;
  while (!fin.eof()) {
    getline(fin, line);
    if (line.empty()) {
      break;
    }

    Tokenize(line, ',', parts);

    std::string curr_value = parts[split_field];

    if (curr_value != prev_value) {
      // Start a new date
      if (fout.good()) {
        fout.close();
      }
      std::string name = base_name + "_" + curr_value;

      VLOG(1) << "Writing file " << name;
      fout.open(name.c_str(), std::ios::out | std::ios::trunc);
    }

    line += "\n";
    fout.write(line.c_str(), line.length());

    prev_value = curr_value;

    if (count % 1000000 == 0) {
      VLOG(1) << "records processed " << count;
    }
    count++;
  }

  if (fout.good()) {
    fout.close();
  }
}
