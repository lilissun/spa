#!/bin/bash

mkdir -p result

for file in `find examples -name '*.cal'`
do
echo "Processing $file"
./spa $file > result/`basename $file`.out
# { timeout 60 ./TAuth $file ; } > result/`basename $file`.to # 2> result/`basename $file`.ti
done