#!/usr/bin/env bash

# Given a list of numbers (one per line), calculates stats on those numbers

echo -e "Sum"'\t\t'"Num Values"'\t'"Average"'\t\t'"Median"'\t\t'"Min"'\t\t'"Max"

sort -n | awk '
  BEGIN {
    c = 0;
    sum = 0;
  }
  $1 ~ /^[0-9]*(\.[0-9]*)?$/ {
    a[c++] = $1;
    sum += $1;
  }
  END {
    ave = sum / c;
    if( (c % 2) == 1 ) {
      median = a[ int(c/2) ];
    } else {
      median = ( a[c/2] + a[c/2-1] ) / 2;
    }
    OFS="\t\t";
    print sum, c, ave, median, a[0], a[c-1];
  }
'