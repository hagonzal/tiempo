// Copyright (c) 2012 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#ifndef UTIL_BITUTILS_H_
#define UTIL_BITUTILS_H_

class BitUtils {
public:
  BitUtils();
  ~BitUtils();

  // returns a Value with bits [low_bit + num_bits - 1, low_bits] set to
  // v[num_bits - 1, 0]
  static inline unsigned long SetBits(unsigned int v, char low_bit,
      char num_bits) {
    unsigned long mask = (1 << num_bits ) - 1;
    return (v & mask) << low_bit;
  }

  // returns the Value in the bits v[low_bit + num_bits - 1, low_bit]
  static inline unsigned long GetBits(unsigned long v, char low_bit,
      char num_bits) {
    unsigned long mask = (1 << num_bits) - 1;
    return (v >> low_bit) & mask;
  }

  static inline unsigned short change_endian(unsigned short x) {
    return ((x & 0xFF00) >> 8) |
           ((x & 0x00FF) << 8);
  }

  static inline unsigned int change_endian(unsigned int x) {
    return ((x & 0xFF000000) >> 24) |
           ((x & 0x00FF0000) >> 8) |
           ((x & 0x0000FF00) << 8) |
           ((x & 0x000000FF) << 24);
  }

  static inline unsigned long change_endian(unsigned long x) {
    return ((x & 0xFF00000000000000) >> 56) |
           ((x & 0x00FF000000000000) >> 40) |
           ((x & 0x0000FF0000000000) >> 24) |
           ((x & 0x000000FF00000000) >> 8) |
           ((x & 0x00000000FF000000) << 8) |
           ((x & 0x0000000000FF0000) << 24) |
           ((x & 0x000000000000FF00) << 40) |
           ((x & 0x00000000000000FF) << 56);
  }

private:
  // Disallow copy and assign
  explicit BitUtils(const BitUtils& copy_from_me);
  void operator=(const BitUtils&);
};

#endif /* UTIL_BITUTILS_H_ */
