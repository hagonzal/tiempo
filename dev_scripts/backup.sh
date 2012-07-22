#!/bin/bash

# Copyright (c) 2011 The Tiempo Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file. See the AUTHORS file for names of contributors.
#
# Author: Hector Gonzalez (hagonzal@gmail.com)
#
# Creates a backup of the code in the input directory

if [ $# -ne 1 ]
then
  echo "Usage: backup.sh dir. From the tiempo directory"
  exit 1
fi

path=`pwd`

if [[ ! $path =~ .*tiempo$ ]]
then
  echo "You must be in the ./tiempo directory. Your path is $path"
  exit 1
fi  

# remove trailing / if present
dir=`echo $1 | sed -e "s/\/*$//"`

name=`date +"%m_%d_%Y_%H_%M"`
name="tiempo.$name.tgz"

echo "make clean"
make clean
cd ..

echo "creating backup $dir/$name"
tar -czf $dir/$name tiempo

cd tiempo
