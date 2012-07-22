// Copyright (c) 2011 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#ifndef UTIL_ENV_H_
#define UTIL_ENV_H_

// Keeps track of the environment: Global variables for the entire program.
class Env {
 public:
  static Env* GetInstance();

 private:
  Env();
  virtual ~Env();

  static Env* env_ ;
};

#endif /* UTIL_ENV_H_ */
