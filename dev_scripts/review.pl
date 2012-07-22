#!/usr/bin/perl
#
# Script to print or remove review comments
# Author: Hector Gonzalez (hagonzal@gmail.com)

use strict;

if ($#ARGV != 1) {
  print "Usage: review.pl file print | remove\n";
  exit;
}

open(FILE, $ARGV[0]) or die("Unable to open file ".$ARGV[0]);

my @lines = <FILE>;

if ($ARGV[1] eq "print") {
  # Print the review comments
  Print($ARGV[0],\@lines, 1);
} elsif ($ARGV[1] eq "remove") {
  # Remove the review comments
  Print($ARGV[0],\@lines, 0);
} else {
  print "Unrecognized command: ".$ARGV[1]."\n";
}

# Prints or removes review comments to stdout
# Arguments
# file_name
# array of text lines
# print_comments: 1 prints, 0 removes
sub Print {
  my($file,$lines,$print_comments) = @_;

  my $in_review = 0;
  my $separator = 0;
  foreach my $i (0..$#lines) {
    my $line = $lines[$i];
    my $next = "";
    if ($i < $#lines - 1) {
      $next = $lines[$i + 1];
    }

    $separator = 0;

    # A review comment is of the form
    # //@ review comment
    # //more review comment
    # a review comment ends on the first non-comment line
    # after the review block
    if ($line =~ /^ *\/\/@/) {
      $in_review = 1;
    }

    if ($in_review) {
      if (!($line =~ /^ *\/\//)) {
        $in_review = 0;
      }
      # Review comments are separated from regular comments
      # by a blank line. Technically this line is still part
      # of the review comment
      if ($line =~ /^ *$/ && $next =~ /^ *\/\/[^@]/) {
        $separator = 1;
      }
    }

    if ($print_comments) {
      if ($in_review) {
        print $file.": ".$i.": ".$line;
      }
    } else {
      if (!$in_review && !$separator) {
        print $line;
      }
    }
  }
}

