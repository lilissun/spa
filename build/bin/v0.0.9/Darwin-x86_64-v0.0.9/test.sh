#!/bin/bash

mkdir -p result

for file in `find examples -name '*.tv'`
do
echo "Processing $file"
./PTAuth $file > result/`basename $file`.to
# { timeout 60 ./TAuth $file ; } > result/`basename $file`.to # 2> result/`basename $file`.ti
done