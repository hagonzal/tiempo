// Copyright (c) 2012 The Tiempo Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Author: Hector Gonzalez (hagonzal@gmail.com)

#ifndef PACKAGE_EXAMPLE_H_
#define PACKAGE_EXAMPLE_H_

class Example {
public:
  Example();
  ~Example();


private:
  // Disallow copy and assign
  explicit Example(const Example& copy_from_me);
  void operator=(const Example&);
};

#endif /* PACKAGE_EXAMPLE_H_ */
