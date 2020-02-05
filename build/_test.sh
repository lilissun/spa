#!/bin/bash
VERSION="0.0.5"

if [[ $OS == "Windows_NT" ]]; then
    ARCH=$PROCESSOR_ARCHITECTURE
else 
    OS=`uname -s`
    ARCH=`uname -m`
fi
TARGET="bin/v$VERSION/$OS-$ARCH-v$VERSION/TAuth"

mkdir -p result
pwd

for file in `find examples -name '*.tv'`
do
echo "Processing $file with $TARGET"
$TARGET $file > result/`basename $file`.to
done