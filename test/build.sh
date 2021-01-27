#!/bin/bash

# exit at first error
set -e

TEST_EXE=test-queue

rm -f $TEST_EXE
rm -rf build/

mkdir build
cd build
cmake ../.

echo
make clean
make

cd ..
echo
echo "++ successfully built:"
stat --printf="%n - %s bytes\n" $TEST_EXE

echo
echo "++ list of tests:"
./$TEST_EXE --gtest_list_tests
echo
