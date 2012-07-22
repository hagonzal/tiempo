#!/bin/sh

# Copyright (c) 2012 The Tiempo Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file. See the AUTHORS file for names of contributors.
#
# Author: Hector Gonzalez (hagonzal@gmail.com)
#
# Prints all review comments found under the current directory
# Review comments start with //@

if [ $# -ne 0 ]
then
  echo "Usage: print_review_comments"
  exit 1
fi  

find . -regex ".*\.\(cc\|h\|proto\)" | xargs -n1 -I {} ./dev_scripts/review.pl {} print
