// Copyright (c) 2011 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#include "util/env.h"

#include <cstddef>
#include <memory>

Env* Env::env_ = NULL;

Env::Env() {
}

Env::~Env() {
}

Env* Env::GetInstance() {
  if (env_ != NULL) {
    return env_;
  }

  env_ = new Env();
  return env_;
}
