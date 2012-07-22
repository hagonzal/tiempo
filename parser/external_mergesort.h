// Copyright (c) 2012 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#ifndef PARSER_EXTERNALMERGESORT_H_
#define PARSER_EXTERNALMERGESORT_H_

#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <queue>

#include <glog/logging.h>

#include "util/timestamp.h"

// TODO(hagonzal): Generalize this class to sort on a column of any type.

// Keeps track of a line in a text file and its key. Used to sort lines.
struct Line {
  Line();
  Line(const std::string& line, int block_id, TimeStamp key);
  ~Line();

  int block_id_;
  TimeStamp key_;
  std::string line_;
};

// Comparator of lines.
class CompareLine {
 public:
  bool operator()(Line& m1, Line& m2) {
    return m1.key_ > m2.key_;
  }
};

// Represents a block of lines. All lines in the block are sorted on the key.
class Block {
public:
  Block(std::string& symbol, int block_id);
  ~Block();

  void SetStartOffset(long offset);
  void SetEndOffset(long offset);

  void SetStartLine(long line);
  void SetEndLine(long line);

  void Open(const std::string& file_name);
  void Close();

  Line Next(int date_field_, int time_field_);

  std::string ToString();
private:
  std::fstream fin_;

  int block_id_;
  int date_field_;
  int time_field_;
  std::string symbol_;

  long start_offset_;
  long end_offset_;

  long start_line_;
  long end_line_;

  int curr_line_;
};

// Globally sorts a set of individually sorted blocks.
class ExternalMergeSort {
public:
  // file_name: the file to sort. One record per line. Fields in the
  // record are separated by comma.
  // date_field_: the offset of the date field (used for sorting)
  // time_field_: the offset of the time field (used for sorting)
  // symbol_field_: the offset of the symbol field. Assumes that each symbol
  // is already sorted on the date/time fields.
  ExternalMergeSort(const std::string& file_name, int date_field_,
      int time_field_, int symbol_field_);
  ~ExternalMergeSort();

  // Sorts the input file on the date/time fields. Outputs the globally
  // sorted output to stdout.
  void SortData();

private:
  void MergeBlocks();
  void ComputeBlocks();

  int date_field_;
  int time_field_;
  int symbol_field_;
  std::string file_name_;

  std::vector<Block *> blocks_;
  std::priority_queue<Line, std::vector<Line>, CompareLine > queue_;
};

#endif /* PARSER_EXTERNALMERGESORT_H_ */
