#!/bin/sh

# Copyright (c) 2012 The Tiempo Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file. See the AUTHORS file for names of contributors.
#
# Author: Hector Gonzalez (hagonzal@gmail.com)
#
# Deletes review comment lines from source files.
# Review comments start with //@

if [ $# -lt 1 ]
then
  echo "Usage: delete_review_comments files"
  exit 1
fi  

file=$1

for var in "$@"
do
  echo "will delete review comments $var"
  ./dev_scripts/review.pl $var remove > $var.tmp
  ./dev_scripts/blank.pl $var.tmp > $var
  rm $var.tmp
done
