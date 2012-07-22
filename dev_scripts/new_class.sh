#!/bin/sh

# Copyright (c) 2012 The Tiempo Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file. See the AUTHORS file for names of contributors.
#
# Author: Hector Gonzalez (hagonzal@gmail.com)
#
# Creates the .h and .cc files for a new class.

if [ $# -ne 3 ]
then
  echo "Usage: new_class.sh class_name file_name package_name"
  echo "Example: new_class.sh SignalValue signal_value signal"
  exit 1
fi  

name=$1
file=$2

# remove trailing / if present
package=`echo $3 | sed -e "s/\/*$//"`

iname=`echo $name | tr '[:lower:]' '[:upper:]'`
lname=`echo $name | tr '[:upper:]' '[:lower:]'`
pname=`echo $package | tr '[:lower:]' '[:upper:]'`

cname="$package/$file.cc"
hname="$package/$file.h"

p="$pname"_"$iname"

e1="s/PACKAGE_EXAMPLE/$p/g"
e2="s/Example/$name/g"
e3="s/package\/example/$package\/$file/g"

echo "will create $cname and $hname"

sed -e $e1 -e $e2 dev_scripts/example.h > $hname
sed -e $e2 -e $e3 dev_scripts/example.cc > $cname
