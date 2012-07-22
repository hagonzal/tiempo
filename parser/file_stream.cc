// Copyright (c) 2012 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#include "parser/file_stream.h"

#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <vector>

#include <glog/logging.h>

#include "util/bit_utils.h"

FileStream::FileStream() {
}

FileStream::~FileStream() {
  fin_.close();
}

void FileStream::open(const char * in_file) {
  std::fstream fin;
  fin.open(in_file, std::ios::in | std::ios::binary);

  if (!fin.good()) {
    LOG(ERROR) << "could not open " << in_file;
    exit(1);
  }
}

unsigned short FileStream::NextShort() {
  fin_.read(buf, 2);
  unsigned short *a = (unsigned short *) buf;
  return BitUtils::change_endian(*a);
}

unsigned int FileStream::NextInt() {
  fin_.read(buf, 4);
  unsigned int *a = (unsigned int *) buf;
  return BitUtils::change_endian(*a);
}

unsigned long FileStream::NextLong() {
  fin_.read(buf, 8);
  unsigned long *a = (unsigned long *) buf;
  return BitUtils::change_endian(*a);
}

double FileStream::NextFixedPoint() {
  fin_.read(buf, 4);

  unsigned int *p = (unsigned int *) buf;
  unsigned int price = BitUtils::change_endian(*p);

  double dprice = (int) (price / 10000) +
      ((double) (price % 10000) / 10000);

  return dprice;
}

char FileStream::NextChar() {
  fin_.read(buf, 1);
  return buf[0];
}

void FileStream::NextString(char *out, int len) {
  fin_.read(out, len);
  out[len] = 0;
}

bool FileStream::eof() {
  return fin_.eof();
}

