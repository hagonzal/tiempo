#!/bin/bash

# Copyright (c) 2011 The Tiempo Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file. See the AUTHORS file for names of contributors.
#
# Author: Hector Gonzalez (hagonzal@gmail.com)
#
# Counts lines of code in .h .cc .proto .sh Makefile files

find . -regex ".*\.\(cc\|h\|proto\|sh\|Makefile\)" | xargs wc -l
