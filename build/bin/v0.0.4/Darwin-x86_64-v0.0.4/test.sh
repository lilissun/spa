#!/bin/bash

mkdir -p out

for file in `find examples -name '*.tv'`
do
echo "Processing $file"
./TAuth $file > out/`basename $file`.to
done