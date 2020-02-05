#!/bin/bash

mkdir -p result

for file in `find examples -name '*.tv'`
do
echo "Processing $file"
# ./TAuth $file > result/`basename $file`.to
{ timeout 6 ./TAuth $file ; } > result/`basename $file`.to # 2> result/`basename $file`.ti
done

for file in `find examples -name '*.tv.ap'`
do
echo "Processing $file"
# ./TAuth $file > result/`basename $file`.to
{ timeout 6 ./TAuth $file ; } > result/`basename $file`.to # 2> result/`basename $file`.ti
done