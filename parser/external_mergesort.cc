// Copyright (c) 2012 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#include "parser/external_mergesort.h"

#include <stdlib.h>
#include <iostream>
#include <sstream>

#include <glog/logging.h>

#include "parser/csv_parser.h"
#include "util/util.h"

ExternalMergeSort::ExternalMergeSort(const std::string& file_name,
    int date_field, int time_field, int symbol_field) {
  file_name_ = file_name;
  date_field_ = date_field;
  time_field_ = time_field;
  symbol_field_ = symbol_field;
}

ExternalMergeSort::~ExternalMergeSort() {
}

void ExternalMergeSort::SortData() {
  ComputeBlocks();
  MergeBlocks();
}

void ExternalMergeSort::ComputeBlocks() {
  std::fstream fin;
  fin.open(file_name_.c_str());
  if (!fin.good()) {
    LOG(ERROR) << "could not find " << file_name_;
    exit(1);
  }

  std::string parts[20];
  std::string line;

  std::string prev_symbol;

  long curr_line = 0;
  long curr_pos = 0;

  while (!fin.eof()) {
    getline(fin, line);
    if (line.empty()) {
      break;
    }

    Tokenize(line, ',', parts);

    std::string symbol = parts[symbol_field_];

    if (symbol != prev_symbol) {
      VLOG(1) << "prev " << prev_symbol << " now " << symbol;

      // New symbol
      Block * block = new Block(symbol, blocks_.size());
      block->SetStartLine(curr_line);
      block->SetStartOffset(curr_pos);
      if (blocks_.size() > 0) {
        blocks_[blocks_.size() - 1]->SetEndLine(curr_line - 1);
        blocks_[blocks_.size() - 1]->SetEndOffset(curr_pos - 1);
      }

      blocks_.push_back(block);
    }
    curr_pos = fin.tellg();
    curr_line++;

    prev_symbol = symbol;
  }

  if (blocks_.size() > 0) {
    blocks_[blocks_.size() - 1]->SetEndLine(curr_line - 1);
    blocks_[blocks_.size() - 1]->SetEndOffset(curr_pos - 1);
  }

  fin.close();

  VLOG(1) << "num blocks " << blocks_.size();
  for (int i = 0; i < blocks_.size(); i++) {
    VLOG(1) << blocks_[i]->ToString();
  }
}

void ExternalMergeSort::MergeBlocks() {
  // Open the blocks, and fill queue
  for (int i = 0; i < blocks_.size(); i++) {
    blocks_[i]->Open(file_name_);
    queue_.push(blocks_[i]->Next(date_field_, time_field_));
  }

  int count = 0;

  while (queue_.size() > 0) {
    Line l = queue_.top();
    queue_.pop();

    std::cout << l.line_ << "\n";

    l = blocks_[l.block_id_]->Next(date_field_, time_field_);
    if (!l.line_.empty()) {
      queue_.push(l);
    }

    if (count % 1000000 == 0) {
      VLOG(1) << "merge progress " << count;
    }
    count++;
  }

  // Close and delete the blocks
  for (int i = 0; i < blocks_.size(); i++) {
    blocks_[i]->Close();
    delete blocks_[i];
  }
}

// Line methods

Line::Line() {
  line_ = "";
  key_ = TimeStamp(0);
}

Line::Line(const std::string& line, int block_id, TimeStamp key) {
  if (line.empty()) {
    return;
  }

  line_ = line;
  block_id_ = block_id;
  key_ = key;
}

Line::~Line() {
}

// Block methods

Block::Block(std::string& symbol, int block_id) {
  symbol_ = symbol;
  block_id_ = block_id;
}

Block::~Block() {
}

void Block::SetStartOffset(long offset) {
  start_offset_ = offset;
}

void Block::SetEndOffset(long offset) {
  end_offset_ = offset;
}

void Block::SetStartLine(long line) {
  start_line_ = line;
}

void Block::SetEndLine(long line) {
  end_line_ = line;
}

void Block::Open(const std::string& file_name) {
  fin_.open(file_name.c_str());
  if (!fin_.good()) {
    LOG(ERROR) << "could not find " << file_name;
    exit(1);
  }

  curr_line_ = start_line_;
  fin_.seekg (start_offset_, std::ios::beg);
}

void Block::Close() {
  fin_.close();
}

Line Block::Next(int date_field_, int time_field_) {
  TimeStamp t;
  if (curr_line_ > end_line_ || fin_.eof()) {
    return Line();
  }

  std::string line;
  getline(fin_, line);
  curr_line_++;

  std::string parts[20];
  Tokenize(line, ',', parts);
  t.ParseDateTime(parts[date_field_], parts[time_field_]);

  //VLOG(1) << "Block Next " << block_id_ << " " << line;

  return Line(line, block_id_, t);
}

std::string Block::ToString() {
  std::stringstream stream;

  stream << "symbol " << symbol_ << " "
      << "block_id " << block_id_ << " "
      << "start_offset " << start_offset_ << " "
      << "end_offset " << end_offset_ << " "
      << "start_line " << start_line_ << " "
      << "end_line " << end_line_;

  return stream.str();
}
