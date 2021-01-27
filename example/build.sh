#!/bin/bash

# exit at first error
set -e

MAIN_EXE=taxicab
OUTPUT_DIR=output

rm -f $MAIN_EXE
rm -rf build/

mkdir build
cd build
cmake ../.

echo
make clean
make

cd ..
mkdir -p $OUTPUT_DIR

echo
echo "++ created directory: ${OUTPUT_DIR}"

echo
echo "++ successfully built:"
stat --printf="%n - %s bytes\n" $MAIN_EXE
echo
