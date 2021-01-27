#!/bin/bash

# exit at first error
set -e

# default args
DEF_BMARK_POW_N=5
DEF_BMARK_POW_R=2
DEF_BMARK_SIZE_T=1
# read args
BMARK_POW_N=${1:-$DEF_BMARK_POW_N}
BMARK_POW_R=${2:-$DEF_BMARK_POW_R}
BMARK_SIZE_T=${3:-$DEF_BMARK_SIZE_T}

BMARK_EXE=bmark-taxicab
OUTPUT_DIR=output

rm -f $BMARK_EXE
rm -rf build/

mkdir build
cd build
export BMARK_POW_N=${BMARK_POW_N} ; export BMARK_POW_R=${BMARK_POW_R} ; export BMARK_SIZE_T=${BMARK_SIZE_T} && cmake ../.

echo
make clean
make

cd ..
mkdir -p $OUTPUT_DIR

echo
echo "++ created directory: ${OUTPUT_DIR}"

echo
echo "++ successfully built:"
stat --printf="%n - %s bytes\n" $BMARK_EXE

echo
echo "++ pow N : $BMARK_POW_N"
echo "++ pow R : $BMARK_POW_R"
echo "++ size T: $BMARK_SIZE_T"
