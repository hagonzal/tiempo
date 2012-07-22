// Copyright (c) 2012 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#include "parser/csv_parser_low.h"

#include <stdio.h>
#include <string.h>

#include <iostream>

CsvParserLow::CsvParserLow(const char *file_name) {
  file_ = fopen (file_name, "r");
  field_count_ = -1;
}

CsvParserLow::~CsvParserLow() {
  fclose(file_);
}

char** tokenize(char *line, int count) {
  char** result = new char*[count];

  int pos = 0;
  int start = 0;
  int i;
  for (i = 0; line[i] != 0; i++) {
    if (line[i] == ',' || line[i] == '\n') {
      int l = i - start;
      result[pos] = new char[l + 1];
      memcpy(result[pos], line + start, sizeof(char) * l);
      result[pos][l] = 0;

      start = i + 1;
      pos++;
    }
  }

  return result;
}

int countFields(char *line) {
  int count = 0;
  for (int i = 0; line[i] != 0; i++) {
    if (line[i] == ',') {
      count++;
    }
  }
  return count + 1;
}

int CsvParserLow::GetFieldCount() {
  return field_count_;
}

char **CsvParserLow::NextLine() {
  char line[1024];
  if (fgets(line, 1024, file_) == NULL) {
    return NULL;
  }

  if (field_count_ == -1) {
    field_count_ = countFields(line);
  }

  return tokenize(line, field_count_);
}
