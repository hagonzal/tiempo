// Copyright (c) 2012 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#ifndef PARSER_BYTESTREAM_H_
#define PARSER_BYTESTREAM_H_

#include <fstream>

// Parses binary and ascii data from a char[] array.
// Useful for parsing ITCH files. See parser/itch_parser.h
// Example use:
// ByteStream stream;
// char* buf = read_from_disk(...)
// stream.reset(buf, size)
// unsigned short a = stream.NextShort();
// char c = stream.NextChar();
class ByteStream {
public:
  ByteStream();
  ~ByteStream();

  // Sets the buffer to use. Sets the size of the buffer.
  void reset(char *buf, int size);

  // Parses a short from the buffer and advances the buffer position by 2.
  unsigned short NextShort();

  // Parses an integer from the buffer and advances the buffer position by 4.
  unsigned int NextInt();

  // Parses a long from the buffer and advances the buffer position by 8.
  unsigned long NextLong();

  // Parses a fixed point value from the buffer. 4 digits integer portion.
  // 4 digits decimal portion. Advances the buffer position by 4.
  double NextFixedPoint();

  // Parses a char array from the buffer and advances the buffer position by
  // len.
  void NextString(char *buf, int len);

  // Parses a char from the buffer and advances the buffer position by 1.
  char NextChar();

  // Returns true if we are at the end of the buffer.
  bool eof();
private:
  // Disallow copy and assign
  explicit ByteStream(const ByteStream& copy_from_me);
  void operator=(const ByteStream&);

  // Buffer currently being parsed.
  char *buf_;

  // Current position in the buffer.
  int pos_;

  // Total size of the buffer.
  int size_;
};

#endif /* PARSER_BYTESTREAM_H_ */
