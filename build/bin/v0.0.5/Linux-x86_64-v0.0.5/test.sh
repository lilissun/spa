#!/bin/bash

mkdir -p result

for file in `find examples -name '*.tv'`
do
echo "Processing $file"
./TAuth $file > result/`basename $file`.to
done