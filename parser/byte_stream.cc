// Copyright (c) 2012 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#include "parser/byte_stream.h"

#include <glog/logging.h>

#include "util/bit_utils.h"

ByteStream::ByteStream() {
}

ByteStream::~ByteStream() {
}

void ByteStream::reset(char *buf, int size) {
  buf_ = buf;
  size_ = size;
  pos_ = 0;
}

unsigned short ByteStream::NextShort() {
  unsigned short *a = (unsigned short *) (buf_ + pos_);
  pos_ += 2;
  return BitUtils::change_endian(*a);
}

unsigned int ByteStream::NextInt() {
  unsigned int *a = (unsigned int *) (buf_ + pos_);
  pos_ += 4;
  return BitUtils::change_endian(*a);
}

unsigned long ByteStream::NextLong() {
  unsigned long *a = (unsigned long *) (buf_ + pos_);
  pos_ += 8;
  return BitUtils::change_endian(*a);
}

double ByteStream::NextFixedPoint() {
  unsigned int *p = (unsigned int *) (buf_ + pos_);
  pos_ += 4;
  unsigned int price = BitUtils::change_endian(*p);

  double dprice = (int) (price / 10000) +
      ((double) (price % 10000) / 10000);

  return dprice;
}

char ByteStream::NextChar() {
  char c = buf_[pos_];
  pos_++;
  return c;
}

void ByteStream::NextString(char *out, int len) {
  memcpy(out, buf_ + pos_, len);
  out[len] = 0;
  pos_ += len;
}
