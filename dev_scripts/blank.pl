#!/usr/bin/perl
#
# Script to remove extra blank lines
# Author: Hector Gonzalez (hagonzal@gmail.com)

use strict;

if ($#ARGV != 0) {
  print "Usage: blank.pl\n";
  exit;
}

open(FILE, $ARGV[0]) or die("Unable to open file");

my @lines = <FILE>;

Print(\@lines);

sub Print {
  my($lines) = @_;

  print $lines[0];

  my $prev = $lines[0];
  foreach my $i (1..$#lines) {
    my $line = $lines[$i];

    if ($prev =~ /^ *$/ && $line =~ /^ *$/) {
      # do not print
    } else {
      print $line;
    }
    $prev = $line;
  }
}

