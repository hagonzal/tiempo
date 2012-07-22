// Copyright (c) 2012 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#ifndef PARSER_FILESTREAM_H_
#define PARSER_FILESTREAM_H_

#include <fstream>

// Parses binary and ascii data from a file.
// See parser/ByteStream for more detailed documentation.
class FileStream {
public:
  FileStream();
  ~FileStream();

  // Opens the given file for parsing.
  void open(const char * in_file);

  // Parses the next 2 bytes as a short.
  unsigned short NextShort();

  // Parses the next 4 bytes as an int.
  unsigned int NextInt();

  // Parses the next 8 bytes as a long.
  unsigned long NextLong();

  // Parses the next 4 bytes as a fixed point number.
  double NextFixedPoint();

  // Parses the next len bytes as a char *.
  void NextString(char *buf, int len);

  // Parses the next 1 byte as a char.
  char NextChar();

  // Returns true if we are at the end of the file.
  bool eof();
private:
  // Disallow copy and assign
  explicit FileStream(const FileStream& copy_from_me);
  void operator=(const FileStream&);

  std::fstream fin_;
  char buf[1024];
};

#endif /* PARSER_FILESTREAM_H_ */
