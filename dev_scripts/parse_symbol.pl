#!/usr/bin/perl
# Author: hagonzal@gmail.com

use strict;
use warnings;

use LWP::Simple;
#use File::Slurp;

if ($#ARGV + 1 != 1 ) {
	print "usage: parse_symbol.pl symbol\n";
  exit;
}

print "Symbol: " . $ARGV[0] . "\n";

my $url = "http://finance.yahoo.com/q?s=" . $ARGV[0];

#my $rc = getstore('http://finance.yahoo.com/q?s=GOOG', $file); die "Failed to download document\n" unless $rc == 200e strict;
#my $text = read_file($file);

my $text = get($url) or die 'Unable to get page';

if ($text =~ /.*Volume:.*?<span.*?>(.*?)<.span>.*/) {
	print "Volume: " . $1 . "\n";
}

if ($text =~ /.*Avg Vol .*?<td.*?>(.*?)<.td>.*/) {
	print "Avg Volume: " . $1 . "\n";
}

if ($text =~ /.*Market Cap:.*?<span.*?>(.*?)<.span>.*/) {
	print "Market Cap: " . $1 . "\n";
}

if ($text =~ /.*Sector.*?http\:\/\/biz.yahoo.com\/p.*?>(.*?)<.a>.*/) {
	print "Sector: " . $1 . "\n";
}

if ($text =~ /.*Industry.*?http\:\/\/biz.yahoo.com\/ic.*?\">(.*?)<.a>.*/) {
	print "Industry: " . $1 . "\n";
}
