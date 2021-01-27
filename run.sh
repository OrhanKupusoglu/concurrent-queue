#!/bin/bash

SEP_1="\n$(printf "%0.s=" {1..80})\n"
SEP_2="\n$(printf "%0.s-" {1..80})\n"

function print_run() {
    echo "$@"
    "$@"
}

printf "$SEP_1"
cd test/
print_run ./build.sh
printf "$SEP_2"
print_run ./test-queue

printf "$SEP_1"
cd ..
cd example/
print_run ./build.sh
printf "$SEP_2"
print_run ./taxicab help
printf "$SEP_2"
print_run ./taxicab

printf "$SEP_1"
cd test
print_run ./build.sh
printf "$SEP_2"
print_run ./test-taxicab

printf "$SEP_1"
cd ..
cd benchmark
print_run ./build.sh
printf "$SEP_2"
print_run ./bmark-taxicab

