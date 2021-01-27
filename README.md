# Concurrent Queue

A thread-safe queue is an extremely useful tool for multi-threaded applications.
[Anthony Williams](https://www.amazon.com/C-Concurrency-Action-Anthony-Williams/dp/1617294691/) published in 2008 an article on implementing such a queue using [Boost](https://www.boost.org/) 1.35.
The current code is an updated version of this public-domain code for C++11.

&nbsp;

## C++ Standard and Compiler Selection

The configuration file [CMakeVersion.txt](./test/CMakeVersion.txt) has variables to select the required C++ standard, **BUILD_CPP_STANDARD**, and compiler, **CMAKE_CXX_COMPILER**.

```
$ cd test/

$ cat CMakeVersion.txt
set(BUILD_MAJOR_VER 0)
set(BUILD_MINOR_VER 1)
set(BUILD_PATCH_VER 1)

set(BUILD_CPP_STANDARD 11)

set(CMAKE_CXX_COMPILER "g++")           # native & container
#set(CMAKE_CXX_COMPILER "g++-10")        # container
#set(CMAKE_CXX_COMPILER "clang++")       # native
#set(CMAKE_CXX_COMPILER "clang++-10")    # container
```

## Build and Test

The thread-safe queue, [ConcurrentQueue](./include/concurrent_queue.h), is built with [CMake](https://cmake.org/) and tested with [Google Test](https://en.wikipedia.org/wiki/Google_Test).

The current compiler is g++ **7.5.0**.

```
$ cd test/

$ ./build.sh
-- The CXX compiler identification is GNU 7.5.0
-- Check for working CXX compiler: /usr/bin/g++
-- Check for working CXX compiler: /usr/bin/g++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
++ Compile definitions:
++ CMake build type: Release
++ C++ standard: 11
++ C++ flags: -O3 -DNDEBUG
-- Found GTest: /usr/lib/libgtest.a
-- Configuring done
-- Generating done
-- Build files have been written to: /home/orhanku/ME/DEV/OK/cpp-utils/concurrent-queue/test/build

Scanning dependencies of target test-queue
[ 33%] Building CXX object CMakeFiles/test-queue.dir/src/main.cpp.o
[ 66%] Building CXX object CMakeFiles/test-queue.dir/src/test_queue.cpp.o
[100%] Linking CXX executable ../test-queue
[100%] Built target test-queue

++ successfully built:
test-queue - 807464 bytes

++ list of tests:
TestConcurrentQueue.
  SizeAndClear
  SumTryPop
  SumWaitAndPop1
  SumWaitAndPop2
  SumWaitAndPop3
  SumWaitAndPopWhile
  SumWaitAndPopWhileWithTimeout1
  SumWaitAndPopWhileWithTimeout2
  SumWaitAndPopWhileWithTimeout3
  SumWaitAndPopWhileWithTimeout4

$ ./test-queue
[==========] Running 10 tests from 1 test case.
[----------] Global test environment set-up.
[----------] 10 tests from TestConcurrentQueue
[ RUN      ] TestConcurrentQueue.SizeAndClear
Queue is not empty.
Queue is empty now.
[       OK ] TestConcurrentQueue.SizeAndClear (0 ms)
[ RUN      ] TestConcurrentQueue.SumTryPop
Sum of numbers read between [1,10] is 55.
[       OK ] TestConcurrentQueue.SumTryPop (5 ms)
[ RUN      ] TestConcurrentQueue.SumWaitAndPop1
Sum of numbers read between [1,10] is 55.
[       OK ] TestConcurrentQueue.SumWaitAndPop1 (2752 ms)
[ RUN      ] TestConcurrentQueue.SumWaitAndPop2
Sum of numbers read between [1,10] * 3 is 165.
[       OK ] TestConcurrentQueue.SumWaitAndPop2 (3002 ms)
[ RUN      ] TestConcurrentQueue.SumWaitAndPop3
	++ detached thread with delay 200ms pushed 27 times
	++ detached thread with delay 100ms pushed 54 times
	++ detached thread with delay 300ms pushed 19 times
Three detached threads pushed in total 100 times.
[       OK ] TestConcurrentQueue.SumWaitAndPop3 (5904 ms)
[ RUN      ] TestConcurrentQueue.SumWaitAndPopWhile
Sum of numbers read between [1,10] is 55.
[       OK ] TestConcurrentQueue.SumWaitAndPopWhile (52 ms)
[ RUN      ] TestConcurrentQueue.SumWaitAndPopWhileWithTimeout1
	-- failed to read value at 6th attempt
	-- failed to read value at 8th attempt
	-- failed to read value at 10th attempt
Sum of numbers read between [1,10] is 28, not 55.
Size of the queue is 3.
[       OK ] TestConcurrentQueue.SumWaitAndPopWhileWithTimeout1 (2752 ms)
[ RUN      ] TestConcurrentQueue.SumWaitAndPopWhileWithTimeout2
	-- failed to read value at 6th attempt
	-- failed to read value at 8th attempt
	-- failed to read value at 10th attempt
	-- failed to read value at 12th attempt
	-- failed to read value at 14th attempt
	-- failed to read value at 15th attempt
Read in 16 attempts.
Sum of numbers read between [1,10] is 55.
[       OK ] TestConcurrentQueue.SumWaitAndPopWhileWithTimeout2 (2752 ms)
[ RUN      ] TestConcurrentQueue.SumWaitAndPopWhileWithTimeout3
	-- failed to read value at 24th attempt
	-- failed to read value at 25th attempt
	-- failed to read value at 27th attempt
	-- failed to read value at 28th attempt
	-- failed to read value at 30th attempt
	-- failed to read value at 31th attempt
	-- failed to read value at 33th attempt
	-- failed to read value at 34th attempt
	-- failed to read value at 36th attempt
	-- failed to read value at 37th attempt
	-- failed to read value at 39th attempt
	-- failed to read value at 40th attempt
	-- failed to read value at 42th attempt
	-- failed to read value at 43th attempt
Read in 44 attempts.
Sum of numbers read between [1,10] * 3 is 165.
[       OK ] TestConcurrentQueue.SumWaitAndPopWhileWithTimeout3 (9005 ms)
[ RUN      ] TestConcurrentQueue.SumWaitAndPopWhileWithTimeout4
	++ detached thread with delay 100ms pushed 58 times
	++ detached thread with delay 500ms pushed 12 times
	++ detached thread with delay 200ms pushed 30 times
Three detached threads pushed in total 100 times.
[       OK ] TestConcurrentQueue.SumWaitAndPopWhileWithTimeout4 (6464 ms)
[----------] 10 tests from TestConcurrentQueue (32688 ms total)

[----------] Global test environment tear-down
[==========] 10 tests from 1 test case ran. (32688 ms total)
[  PASSED  ] 10 tests.
```

## Docker

A [Docker](https://en.wikipedia.org/wiki/Docker_(software)) image is prepared with all the requirements and [g++-10](https://en.wikipedia.org/wiki/GNU_Compiler_Collection) and [clang++-10](https://en.wikipedia.org/wiki/Clang), so that even with all the missing requirements, if the Docker runtime is available, the code still can be built and tested with the given C++ standard and compiler.

The following shell script creates a Docker image using a custom [Dockerfile](./Dockerfile) with g++-10, clang++-10, Google Test and [Google Benchmark](https://github.com/google/benchmark) installed.

Ubuntu 20 installs by default g++ version **9.3.0**.

```
$ ./docker_build.sh
```

The Docker container mounts the current repository as **/usr/src/concurrent-queue**, therefore the Docker image creates the current user in the image so that there won't be any file owner errors later on.

Since the Docker container is using the exact same files, the files can be browsed outside of the container.

The following shell script launches an interactive terminal for the container, note that there is an argument: `/docker_run.sh it`

```
$ ./docker_run.sh it
orhanku@630a467076de:/usr/src/concurrent-queue$ cd test/
orhanku@630a467076de:/usr/src/concurrent-queue/test$ ./build.sh
-- The CXX compiler identification is GNU 9.3.0
-- Check for working CXX compiler: /usr/bin/g++
-- Check for working CXX compiler: /usr/bin/g++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
++ Compile definitions:
++ CMake build type: Release
++ C++ standard: 11
++ C++ flags: -O3 -DNDEBUG
-- Found GTest: /usr/local/lib/libgtest.a
-- Configuring done
-- Generating done
-- Build files have been written to: /usr/src/concurrent-queue/test/build

Scanning dependencies of target test-queue
[ 33%] Building CXX object CMakeFiles/test-queue.dir/src/main.cpp.o
[ 66%] Building CXX object CMakeFiles/test-queue.dir/src/test_queue.cpp.o
[100%] Linking CXX executable ../test-queue
[100%] Built target test-queue

++ successfully built:
test-queue - 1070064 bytes

++ list of tests:
TestConcurrentQueue.
  SizeAndClear
  SumTryPop
  SumWaitAndPop1
  SumWaitAndPop2
  SumWaitAndPop3
  SumWaitAndPopWhile
  SumWaitAndPopWhileWithTimeout1
  SumWaitAndPopWhileWithTimeout2
  SumWaitAndPopWhileWithTimeout3
  SumWaitAndPopWhileWithTimeout4

orhanku@630a467076de:/usr/src/concurrent-queue/test$ ./test-queue
[==========] Running 10 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 10 tests from TestConcurrentQueue
[ RUN      ] TestConcurrentQueue.SizeAndClear
Queue is not empty.
Queue is empty now.
[       OK ] TestConcurrentQueue.SizeAndClear (0 ms)
[ RUN      ] TestConcurrentQueue.SumTryPop
Sum of numbers read between [1,10] is 55.
[       OK ] TestConcurrentQueue.SumTryPop (5 ms)
[ RUN      ] TestConcurrentQueue.SumWaitAndPop1
Sum of numbers read between [1,10] is 55.
[       OK ] TestConcurrentQueue.SumWaitAndPop1 (2752 ms)
[ RUN      ] TestConcurrentQueue.SumWaitAndPop2
Sum of numbers read between [1,10] * 3 is 165.
[       OK ] TestConcurrentQueue.SumWaitAndPop2 (3003 ms)
[ RUN      ] TestConcurrentQueue.SumWaitAndPop3
	++ detached thread with delay 200ms pushed 27 times
	++ detached thread with delay 100ms pushed 54 times
	++ detached thread with delay 300ms pushed 19 times
Three detached threads pushed in total 100 times.
[       OK ] TestConcurrentQueue.SumWaitAndPop3 (5904 ms)
[ RUN      ] TestConcurrentQueue.SumWaitAndPopWhile
Sum of numbers read between [1,10] is 55.
[       OK ] TestConcurrentQueue.SumWaitAndPopWhile (51 ms)
[ RUN      ] TestConcurrentQueue.SumWaitAndPopWhileWithTimeout1
	-- failed to read value at 6th attempt
	-- failed to read value at 8th attempt
	-- failed to read value at 10th attempt
Sum of numbers read between [1,10] is 28, not 55.
Size of the queue is 3.
[       OK ] TestConcurrentQueue.SumWaitAndPopWhileWithTimeout1 (2753 ms)
[ RUN      ] TestConcurrentQueue.SumWaitAndPopWhileWithTimeout2
	-- failed to read value at 6th attempt
	-- failed to read value at 8th attempt
	-- failed to read value at 10th attempt
	-- failed to read value at 12th attempt
	-- failed to read value at 14th attempt
	-- failed to read value at 15th attempt
Read in 16 attempts.
Sum of numbers read between [1,10] is 55.
[       OK ] TestConcurrentQueue.SumWaitAndPopWhileWithTimeout2 (2752 ms)
[ RUN      ] TestConcurrentQueue.SumWaitAndPopWhileWithTimeout3
	-- failed to read value at 24th attempt
	-- failed to read value at 25th attempt
	-- failed to read value at 27th attempt
	-- failed to read value at 28th attempt
	-- failed to read value at 30th attempt
	-- failed to read value at 31th attempt
	-- failed to read value at 33th attempt
	-- failed to read value at 34th attempt
	-- failed to read value at 36th attempt
	-- failed to read value at 37th attempt
	-- failed to read value at 39th attempt
	-- failed to read value at 40th attempt
	-- failed to read value at 42th attempt
	-- failed to read value at 43th attempt
Read in 44 attempts.
Sum of numbers read between [1,10] * 3 is 165.
[       OK ] TestConcurrentQueue.SumWaitAndPopWhileWithTimeout3 (9003 ms)
[ RUN      ] TestConcurrentQueue.SumWaitAndPopWhileWithTimeout4
	++ detached thread with delay 100ms pushed 58 times
	++ detached thread with delay 500ms pushed 12 times
	++ detached thread with delay 200ms pushed 30 times
Three detached threads pushed in total 100 times.
[       OK ] TestConcurrentQueue.SumWaitAndPopWhileWithTimeout4 (6466 ms)
[----------] 10 tests from TestConcurrentQueue (32690 ms total)

[----------] Global test environment tear-down
[==========] 10 tests from 1 test suite ran. (32690 ms total)
[  PASSED  ] 10 tests.
orhanku@630a467076de:/usr/src/concurrent-queue/test$ exit
exit
```

&nbsp;

## Taxicab Numbers

As a practical application of the ConcurrentQueue [taxicab numbers](https://en.wikipedia.org/wiki/Taxicab_number) are calculated with multiple threads in a [brute-force](https://en.wikipedia.org/wiki/Brute-force_search) approach.

> In mathematics, the nth **taxicab number**, typically denoted Ta(*n*) or Taxicab(*n*), also called the nth **Hardy–Ramanujan number**, is defined as the smallest integer that can be expressed as a sum of two positive integer cubes in n distinct ways. The most famous taxicab number is 1729 = Ta(*2*) = 1<sup>3</sup> + 12<sup>3</sup> = 9<sup>3</sup> + 10<sup>3</sup>.

### Cubes

The calculated cubes of a taxicab number are stored either in a delimited **std::string** or in the bits of an **uint64_t**.

Check the [CMakeLists.txt](./example/CMakeLists.txt) file for the define *SOLUTION_INT*.

The **std::string** example: cubes of 1729 are `1.12.9.10`

The **uint64_t** example: cubes of 1729 are within `10746867713`
```
binary:  0000 | 0000000000 | 0000000000 | 0000001010 | 0000001001 | 0000001100 | 0000000001
decimal:    0 |          0 |          0 |         10 |          9 |         12 |          1
```

The application writes calculated taxicab numbers either in JSON (default) or TXT files.

### Sample Application

```
$ cd example/

$ ls
benchmark  build.sh  CMakeLists.txt  CMakeVersion.txt  include  src  test

$ ./build.sh
-- The CXX compiler identification is GNU 7.5.0
-- Check for working CXX compiler: /usr/bin/g++
-- Check for working CXX compiler: /usr/bin/g++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
++ Compile definitions: APP_VERSION="0.1.1";CXX_COMPILER="g++";CXX_STANDARD="c++11"
++ CMake build type: Release
++ C++ standard: 11
++ C++ flags: -O3 -DNDEBUG
++ will use __builtin_clzll(x) to find the number of leading 0-bits in x, starting at the most significant bit position
-- Looking for C++ include pthread.h
-- Looking for C++ include pthread.h - found
-- Looking for pthread_create
-- Looking for pthread_create - not found
-- Check if compiler accepts -pthread
-- Check if compiler accepts -pthread - yes
-- Found Threads: TRUE
-- Configuring done
-- Generating done
-- Build files have been written to: /home/orhanku/ME/DEV/OK/cpp-utils/concurrent-queue/example/build

Scanning dependencies of target taxicab
[ 25%] Building CXX object CMakeFiles/taxicab.dir/src/main.cpp.o
[ 50%] Building CXX object CMakeFiles/taxicab.dir/src/taxicab_number.cpp.o
[ 75%] Building CXX object CMakeFiles/taxicab.dir/src/utility.cpp.o
[100%] Linking CXX executable ../taxicab
[100%] Built target taxicab

++ created directory: output

++ successfully built:
taxicab - 80936 bytes

$ ./taxicab help
usage:
	./taxicab 5 2 1
	./taxicab p r t
	p: gives the range for taxicab numbers [1, 10^p]
	r: gives the range for cubes [1, 10^r]
	t: gives the number of producer threads
version: 0.1.1
compiler: g++
standard: c++11
hardware threads: 4

$ ./taxicab
                  taxicab range N = 100000
             integer cube range R = 100
       number of worker threads T = 1
output/str_2021-01-27_15-17-11.json

$ cat output/str_2021-01-27_15-17-11.json
{"meta":{"n":100000,"r":100,"t":1,"total_cubes":938,"total_taxicabs":10,"elapsed_time":"00:00:01.104"},
"list":[
{"taxicab":1729,"cubes":[[1, 12],[9, 10]]},
{"taxicab":4104,"cubes":[[2, 16],[9, 15]]},
{"taxicab":13832,"cubes":[[2, 24],[18, 20]]},
{"taxicab":20683,"cubes":[[10, 27],[19, 24]]},
{"taxicab":32832,"cubes":[[4, 32],[18, 30]]},
{"taxicab":39312,"cubes":[[2, 34],[15, 33]]},
{"taxicab":40033,"cubes":[[9, 34],[16, 33]]},
{"taxicab":46683,"cubes":[[3, 36],[27, 30]]},
{"taxicab":64232,"cubes":[[17, 39],[26, 36]]},
{"taxicab":65728,"cubes":[[12, 40],[31, 33]]}
]}
```
#### Google Test

Calculated numbers can be checked with Google Test against the reference numbers taken from [Numbers Aplenty](https://www.numbersaplenty.com/set/taxicab_number/more.php).

For tests with N=10<sup>7</sup> or more the  define **TEST_XLARGE** must be given in the CMakeLists.txt file, but beware that on a slow computer it may well take many hours to find all the taxicab numbers.

#### Google Benchmark

Since there are two choices to store the cubes, **std::string** or **uint64_t**, a Google Benchmark between these solutions can be run.

## Run Everything

Here all codes are built and run by C++20 and clang++-10 in a Docker container, note that the shell script receives no arguments at all.

```
$ ./docker_run.sh

================================================================================
./build.sh
-- The CXX compiler identification is Clang 10.0.1
-- Check for working CXX compiler: /usr/bin/clang++-10
-- Check for working CXX compiler: /usr/bin/clang++-10 -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
++ Compile definitions:
++ CMake build type: Release
++ C++ standard: 20
++ C++ flags: -O3 -DNDEBUG
-- Found GTest: /usr/local/lib/libgtest.a
-- Configuring done
-- Generating done
-- Build files have been written to: /usr/src/concurrent-queue/test/build

Scanning dependencies of target test-queue
[ 33%] Building CXX object CMakeFiles/test-queue.dir/src/main.cpp.o
[ 66%] Building CXX object CMakeFiles/test-queue.dir/src/test_queue.cpp.o
[100%] Linking CXX executable ../test-queue
[100%] Built target test-queue

++ successfully built:
test-queue - 1022256 bytes

++ list of tests:
TestConcurrentQueue.
  SizeAndClear
  SumTryPop
  SumWaitAndPop1
  SumWaitAndPop2
  SumWaitAndPop3
  SumWaitAndPopWhile
  SumWaitAndPopWhileWithTimeout1
  SumWaitAndPopWhileWithTimeout2
  SumWaitAndPopWhileWithTimeout3
  SumWaitAndPopWhileWithTimeout4


--------------------------------------------------------------------------------
./test-queue
[==========] Running 10 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 10 tests from TestConcurrentQueue
[ RUN      ] TestConcurrentQueue.SizeAndClear
Queue is not empty.
Queue is empty now.
[       OK ] TestConcurrentQueue.SizeAndClear (0 ms)
[ RUN      ] TestConcurrentQueue.SumTryPop
Sum of numbers read between [1,10] is 55.
[       OK ] TestConcurrentQueue.SumTryPop (5 ms)
[ RUN      ] TestConcurrentQueue.SumWaitAndPop1
Sum of numbers read between [1,10] is 55.
[       OK ] TestConcurrentQueue.SumWaitAndPop1 (2752 ms)
[ RUN      ] TestConcurrentQueue.SumWaitAndPop2
Sum of numbers read between [1,10] * 3 is 165.
[       OK ] TestConcurrentQueue.SumWaitAndPop2 (3002 ms)
[ RUN      ] TestConcurrentQueue.SumWaitAndPop3
	++ detached thread with delay 200ms pushed 27 times
	++ detached thread with delay 100ms pushed 54 times
	++ detached thread with delay 300ms pushed 19 times
Three detached threads pushed in total 100 times.
[       OK ] TestConcurrentQueue.SumWaitAndPop3 (5905 ms)
[ RUN      ] TestConcurrentQueue.SumWaitAndPopWhile
Sum of numbers read between [1,10] is 55.
[       OK ] TestConcurrentQueue.SumWaitAndPopWhile (52 ms)
[ RUN      ] TestConcurrentQueue.SumWaitAndPopWhileWithTimeout1
	-- failed to read value at 6th attempt
	-- failed to read value at 8th attempt
	-- failed to read value at 10th attempt
Sum of numbers read between [1,10] is 28, not 55.
Size of the queue is 3.
[       OK ] TestConcurrentQueue.SumWaitAndPopWhileWithTimeout1 (2752 ms)
[ RUN      ] TestConcurrentQueue.SumWaitAndPopWhileWithTimeout2
	-- failed to read value at 6th attempt
	-- failed to read value at 8th attempt
	-- failed to read value at 10th attempt
	-- failed to read value at 12th attempt
	-- failed to read value at 14th attempt
	-- failed to read value at 15th attempt
Read in 16 attempts.
Sum of numbers read between [1,10] is 55.
[       OK ] TestConcurrentQueue.SumWaitAndPopWhileWithTimeout2 (2752 ms)
[ RUN      ] TestConcurrentQueue.SumWaitAndPopWhileWithTimeout3
	-- failed to read value at 24th attempt
	-- failed to read value at 25th attempt
	-- failed to read value at 27th attempt
	-- failed to read value at 28th attempt
	-- failed to read value at 30th attempt
	-- failed to read value at 31th attempt
	-- failed to read value at 33th attempt
	-- failed to read value at 34th attempt
	-- failed to read value at 36th attempt
	-- failed to read value at 37th attempt
	-- failed to read value at 39th attempt
	-- failed to read value at 40th attempt
	-- failed to read value at 42th attempt
	-- failed to read value at 43th attempt
Read in 44 attempts.
Sum of numbers read between [1,10] * 3 is 165.
[       OK ] TestConcurrentQueue.SumWaitAndPopWhileWithTimeout3 (9003 ms)
[ RUN      ] TestConcurrentQueue.SumWaitAndPopWhileWithTimeout4
	++ detached thread with delay 100ms pushed 58 times
	++ detached thread with delay 500ms pushed 12 times
	++ detached thread with delay 200ms pushed 30 times
Three detached threads pushed in total 100 times.
[       OK ] TestConcurrentQueue.SumWaitAndPopWhileWithTimeout4 (6466 ms)
[----------] 10 tests from TestConcurrentQueue (32690 ms total)

[----------] Global test environment tear-down
[==========] 10 tests from 1 test suite ran. (32690 ms total)
[  PASSED  ] 10 tests.

================================================================================
./build.sh
-- The CXX compiler identification is Clang 10.0.1
-- Check for working CXX compiler: /usr/bin/clang++-10
-- Check for working CXX compiler: /usr/bin/clang++-10 -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
++ Compile definitions: APP_VERSION="0.1.1";CXX_COMPILER="clang++-10";CXX_STANDARD="c++20"
++ CMake build type: Release
++ C++ standard: 20
++ C++ flags: -O3 -DNDEBUG
++ will use std::countl_zero(x) to find the number of consecutive 0 bits in the value of x, starting from the most significant bit
-- Looking for C++ include pthread.h
-- Looking for C++ include pthread.h - found
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Failed
-- Check if compiler accepts -pthread
-- Check if compiler accepts -pthread - yes
-- Found Threads: TRUE
-- Configuring done
-- Generating done
-- Build files have been written to: /usr/src/concurrent-queue/example/build

Scanning dependencies of target taxicab
[ 25%] Building CXX object CMakeFiles/taxicab.dir/src/main.cpp.o
[ 50%] Building CXX object CMakeFiles/taxicab.dir/src/taxicab_number.cpp.o
[ 75%] Building CXX object CMakeFiles/taxicab.dir/src/utility.cpp.o
[100%] Linking CXX executable ../taxicab
[100%] Built target taxicab

++ created directory: output

++ successfully built:
taxicab - 79544 bytes


--------------------------------------------------------------------------------
./taxicab help
usage:
	./taxicab 5 2 1
	./taxicab p r t
	p: gives the range for taxicab numbers [1, 10^p]
	r: gives the range for cubes [1, 10^r]
	t: gives the number of producer threads
version: 0.1.1
compiler: clang++-10
standard: c++20
hardware threads: 4

--------------------------------------------------------------------------------
./taxicab
                  taxicab range N = 100000
             integer cube range R = 100
       number of worker threads T = 1
output/str_2021-01-27_16-21-48.json

================================================================================
./build.sh
-- The CXX compiler identification is Clang 10.0.1
-- Check for working CXX compiler: /usr/bin/clang++-10
-- Check for working CXX compiler: /usr/bin/clang++-10 -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
++ Compile definitions: APP_VERSION="0.1.1";CXX_COMPILER="clang++-10";CXX_STANDARD="c++20"
++ CMake build type: Release
++ C++ standard: 20
++ C++ flags: -O3 -DNDEBUG
++ will use std::countl_zero(x) to find the number of consecutive 0 bits in the value of x, starting from the most significant bit
-- Found GTest: /usr/local/lib/libgtest.a
-- Configuring done
-- Generating done
-- Build files have been written to: /usr/src/concurrent-queue/example/test/build

Scanning dependencies of target test-taxicab
[ 12%] Building CXX object CMakeFiles/test-taxicab.dir/usr/src/concurrent-queue/example/src/taxicab_number.cpp.o
[ 25%] Building CXX object CMakeFiles/test-taxicab.dir/usr/src/concurrent-queue/example/src/utility.cpp.o
[ 37%] Building CXX object CMakeFiles/test-taxicab.dir/src/main.cpp.o
[ 50%] Building CXX object CMakeFiles/test-taxicab.dir/src/ref_taxicab.cpp.o
[ 62%] Building CXX object CMakeFiles/test-taxicab.dir/src/test_util.cpp.o
[ 75%] Building CXX object CMakeFiles/test-taxicab.dir/src/test_int.cpp.o
[ 87%] Building CXX object CMakeFiles/test-taxicab.dir/src/test_str.cpp.o
[100%] Linking CXX executable ../test-taxicab
[100%] Built target test-taxicab

++ created directory: output

++ successfully built:
test-taxicab - 1131576 bytes

++ list of tests:
TestTaxiCabInt.
  SizePow1
  SizePow2
  SizePow3
  SizePow4
  SizePow5
  SizePow6
TestTaxiCabStr.
  SizePow1
  SizePow2
  SizePow3
  SizePow4
  SizePow5
  SizePow6


--------------------------------------------------------------------------------
./test-taxicab
[==========] Running 12 tests from 2 test suites.
[----------] Global test environment set-up.
[----------] 6 tests from TestTaxiCabInt
[ RUN      ] TestTaxiCabInt.SizePow1
test taxicab_int with n = 10^1, r = 10^1, t = 1
output/int_2021-01-27_16-22-09.json
found - size = 0
[       OK ] TestTaxiCabInt.SizePow1 (1011 ms)
[ RUN      ] TestTaxiCabInt.SizePow2
test taxicab_int with n = 10^2, r = 10^1, t = 1
output/int_2021-01-27_16-22-10.json
found - size = 0
[       OK ] TestTaxiCabInt.SizePow2 (1012 ms)
[ RUN      ] TestTaxiCabInt.SizePow3
test taxicab_int with n = 10^3, r = 10^2, t = 1
output/int_2021-01-27_16-22-11.json
found - size = 0
[       OK ] TestTaxiCabInt.SizePow3 (1053 ms)
[ RUN      ] TestTaxiCabInt.SizePow4
test taxicab_int with n = 10^4, r = 10^2, t = 1
output/int_2021-01-27_16-22-13.json
found - size = 2
	1729 = 1^3 + 12^3
	1729 = 9^3 + 10^3
	4104 = 2^3 + 16^3
	4104 = 9^3 + 15^3
[       OK ] TestTaxiCabInt.SizePow4 (1139 ms)
[ RUN      ] TestTaxiCabInt.SizePow5
test taxicab_int with n = 10^5, r = 10^2, t = 1
output/int_2021-01-27_16-22-15.json
found - size = 10
	1729 = 1^3 + 12^3
	1729 = 9^3 + 10^3
	4104 = 2^3 + 16^3
	4104 = 9^3 + 15^3
	13832 = 2^3 + 24^3
	13832 = 18^3 + 20^3
	20683 = 10^3 + 27^3
	20683 = 19^3 + 24^3
	32832 = 4^3 + 32^3
	32832 = 18^3 + 30^3
	39312 = 2^3 + 34^3
	39312 = 15^3 + 33^3
	40033 = 9^3 + 34^3
	40033 = 16^3 + 33^3
	46683 = 3^3 + 36^3
	46683 = 27^3 + 30^3
	64232 = 17^3 + 39^3
	64232 = 26^3 + 36^3
	65728 = 12^3 + 40^3
	65728 = 31^3 + 33^3
[       OK ] TestTaxiCabInt.SizePow5 (2137 ms)
[ RUN      ] TestTaxiCabInt.SizePow6
test taxicab_int with n = 10^6, r = 10^2, t = 1
output/int_2021-01-27_16-22-27.json
found - size = 43
	1729 = 1^3 + 12^3
	1729 = 9^3 + 10^3
	4104 = 2^3 + 16^3
	4104 = 9^3 + 15^3
	13832 = 2^3 + 24^3
	13832 = 18^3 + 20^3
	20683 = 10^3 + 27^3
	20683 = 19^3 + 24^3
	32832 = 4^3 + 32^3
	32832 = 18^3 + 30^3
	39312 = 2^3 + 34^3
	39312 = 15^3 + 33^3
	40033 = 9^3 + 34^3
	40033 = 16^3 + 33^3
	46683 = 3^3 + 36^3
	46683 = 27^3 + 30^3
	64232 = 17^3 + 39^3
	64232 = 26^3 + 36^3
	65728 = 12^3 + 40^3
	65728 = 31^3 + 33^3
	110656 = 4^3 + 48^3
	110656 = 36^3 + 40^3
	110808 = 6^3 + 48^3
	110808 = 27^3 + 45^3
	134379 = 12^3 + 51^3
	134379 = 38^3 + 43^3
	149389 = 8^3 + 53^3
	149389 = 29^3 + 50^3
	165464 = 20^3 + 54^3
	165464 = 38^3 + 48^3
	171288 = 17^3 + 55^3
	171288 = 24^3 + 54^3
	195841 = 9^3 + 58^3
	195841 = 22^3 + 57^3
	216027 = 3^3 + 60^3
	216027 = 22^3 + 59^3
	216125 = 5^3 + 60^3
	216125 = 45^3 + 50^3
	262656 = 8^3 + 64^3
	262656 = 36^3 + 60^3
	314496 = 4^3 + 68^3
	314496 = 30^3 + 66^3
	320264 = 18^3 + 68^3
	320264 = 32^3 + 66^3
	327763 = 30^3 + 67^3
	327763 = 51^3 + 58^3
	373464 = 6^3 + 72^3
	373464 = 54^3 + 60^3
	402597 = 42^3 + 69^3
	402597 = 56^3 + 61^3
	439101 = 5^3 + 76^3
	439101 = 48^3 + 69^3
	443889 = 17^3 + 76^3
	443889 = 38^3 + 73^3
	513000 = 10^3 + 80^3
	513000 = 45^3 + 75^3
	513856 = 34^3 + 78^3
	513856 = 52^3 + 72^3
	515375 = 15^3 + 80^3
	515375 = 54^3 + 71^3
	525824 = 24^3 + 80^3
	525824 = 62^3 + 66^3
	558441 = 30^3 + 81^3
	558441 = 57^3 + 72^3
	593047 = 7^3 + 84^3
	593047 = 63^3 + 70^3
	684019 = 51^3 + 82^3
	684019 = 64^3 + 75^3
	704977 = 2^3 + 89^3
	704977 = 41^3 + 86^3
	805688 = 11^3 + 93^3
	805688 = 30^3 + 92^3
	842751 = 23^3 + 94^3
	842751 = 63^3 + 84^3
	885248 = 8^3 + 96^3
	885248 = 72^3 + 80^3
	886464 = 12^3 + 96^3
	886464 = 54^3 + 90^3
	920673 = 20^3 + 97^3
	920673 = 33^3 + 96^3
	955016 = 24^3 + 98^3
	955016 = 63^3 + 89^3
	984067 = 35^3 + 98^3
	984067 = 59^3 + 92^3
	994688 = 29^3 + 99^3
	994688 = 60^3 + 92^3
[       OK ] TestTaxiCabInt.SizePow6 (12068 ms)
[----------] 6 tests from TestTaxiCabInt (18421 ms total)

[----------] 6 tests from TestTaxiCabStr
[ RUN      ] TestTaxiCabStr.SizePow1
test taxicab_str with n = 10^1, r = 10^1, t = 1
output/str_2021-01-27_16-22-28.json
found - size = 0
[       OK ] TestTaxiCabStr.SizePow1 (1012 ms)
[ RUN      ] TestTaxiCabStr.SizePow2
test taxicab_str with n = 10^2, r = 10^1, t = 1
output/str_2021-01-27_16-22-29.json
found - size = 0
[       OK ] TestTaxiCabStr.SizePow2 (1012 ms)
[ RUN      ] TestTaxiCabStr.SizePow3
test taxicab_str with n = 10^3, r = 10^2, t = 1
output/str_2021-01-27_16-22-30.json
found - size = 0
[       OK ] TestTaxiCabStr.SizePow3 (1054 ms)
[ RUN      ] TestTaxiCabStr.SizePow4
test taxicab_str with n = 10^4, r = 10^2, t = 1
output/str_2021-01-27_16-22-31.json
found - size = 2
	1729 = 1^3 + 12^3
	1729 = 9^3 + 10^3
	4104 = 2^3 + 16^3
	4104 = 9^3 + 15^3
[       OK ] TestTaxiCabStr.SizePow4 (1158 ms)
[ RUN      ] TestTaxiCabStr.SizePow5
test taxicab_str with n = 10^5, r = 10^2, t = 1
output/str_2021-01-27_16-22-33.json
found - size = 10
	1729 = 1^3 + 12^3
	1729 = 9^3 + 10^3
	4104 = 2^3 + 16^3
	4104 = 9^3 + 15^3
	13832 = 2^3 + 24^3
	13832 = 18^3 + 20^3
	20683 = 10^3 + 27^3
	20683 = 19^3 + 24^3
	32832 = 4^3 + 32^3
	32832 = 18^3 + 30^3
	39312 = 2^3 + 34^3
	39312 = 15^3 + 33^3
	40033 = 9^3 + 34^3
	40033 = 16^3 + 33^3
	46683 = 3^3 + 36^3
	46683 = 27^3 + 30^3
	64232 = 17^3 + 39^3
	64232 = 26^3 + 36^3
	65728 = 12^3 + 40^3
	65728 = 31^3 + 33^3
[       OK ] TestTaxiCabStr.SizePow5 (2141 ms)
[ RUN      ] TestTaxiCabStr.SizePow6
test taxicab_str with n = 10^6, r = 10^2, t = 1
output/str_2021-01-27_16-22-45.json
found - size = 43
	1729 = 1^3 + 12^3
	1729 = 9^3 + 10^3
	4104 = 2^3 + 16^3
	4104 = 9^3 + 15^3
	13832 = 2^3 + 24^3
	13832 = 18^3 + 20^3
	20683 = 10^3 + 27^3
	20683 = 19^3 + 24^3
	32832 = 4^3 + 32^3
	32832 = 18^3 + 30^3
	39312 = 2^3 + 34^3
	39312 = 15^3 + 33^3
	40033 = 9^3 + 34^3
	40033 = 16^3 + 33^3
	46683 = 3^3 + 36^3
	46683 = 27^3 + 30^3
	64232 = 17^3 + 39^3
	64232 = 26^3 + 36^3
	65728 = 12^3 + 40^3
	65728 = 31^3 + 33^3
	110656 = 4^3 + 48^3
	110656 = 36^3 + 40^3
	110808 = 6^3 + 48^3
	110808 = 27^3 + 45^3
	134379 = 12^3 + 51^3
	134379 = 38^3 + 43^3
	149389 = 8^3 + 53^3
	149389 = 29^3 + 50^3
	165464 = 20^3 + 54^3
	165464 = 38^3 + 48^3
	171288 = 17^3 + 55^3
	171288 = 24^3 + 54^3
	195841 = 9^3 + 58^3
	195841 = 22^3 + 57^3
	216027 = 3^3 + 60^3
	216027 = 22^3 + 59^3
	216125 = 5^3 + 60^3
	216125 = 45^3 + 50^3
	262656 = 8^3 + 64^3
	262656 = 36^3 + 60^3
	314496 = 4^3 + 68^3
	314496 = 30^3 + 66^3
	320264 = 18^3 + 68^3
	320264 = 32^3 + 66^3
	327763 = 30^3 + 67^3
	327763 = 51^3 + 58^3
	373464 = 6^3 + 72^3
	373464 = 54^3 + 60^3
	402597 = 42^3 + 69^3
	402597 = 56^3 + 61^3
	439101 = 5^3 + 76^3
	439101 = 48^3 + 69^3
	443889 = 17^3 + 76^3
	443889 = 38^3 + 73^3
	513000 = 10^3 + 80^3
	513000 = 45^3 + 75^3
	513856 = 34^3 + 78^3
	513856 = 52^3 + 72^3
	515375 = 15^3 + 80^3
	515375 = 54^3 + 71^3
	525824 = 24^3 + 80^3
	525824 = 62^3 + 66^3
	558441 = 30^3 + 81^3
	558441 = 57^3 + 72^3
	593047 = 7^3 + 84^3
	593047 = 63^3 + 70^3
	684019 = 51^3 + 82^3
	684019 = 64^3 + 75^3
	704977 = 2^3 + 89^3
	704977 = 41^3 + 86^3
	805688 = 11^3 + 93^3
	805688 = 30^3 + 92^3
	842751 = 23^3 + 94^3
	842751 = 63^3 + 84^3
	885248 = 8^3 + 96^3
	885248 = 72^3 + 80^3
	886464 = 12^3 + 96^3
	886464 = 54^3 + 90^3
	920673 = 20^3 + 97^3
	920673 = 33^3 + 96^3
	955016 = 24^3 + 98^3
	955016 = 63^3 + 89^3
	984067 = 35^3 + 98^3
	984067 = 59^3 + 92^3
	994688 = 29^3 + 99^3
	994688 = 60^3 + 92^3
[       OK ] TestTaxiCabStr.SizePow6 (11964 ms)
[----------] 6 tests from TestTaxiCabStr (18341 ms total)

[----------] Global test environment tear-down
[==========] 12 tests from 2 test suites ran. (36762 ms total)
[  PASSED  ] 12 tests.

================================================================================
./build.sh
-- The CXX compiler identification is Clang 10.0.1
-- Check for working CXX compiler: /usr/bin/clang++-10
-- Check for working CXX compiler: /usr/bin/clang++-10 -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
++ Compile definitions: APP_VERSION="0.1.1";CXX_COMPILER="clang++-10";CXX_STANDARD="c++20";BMARK_POW_N=5;BMARK_POW_R=2;BMARK_SIZE_T=1
++ CMake build type: Release
++ C++ standard: 20
++ C++ flags: -O3 -DNDEBUG
++ will use std::countl_zero(x) to find the number of consecutive 0 bits in the value of x, starting from the most significant bit
-- Configuring done
-- Generating done
-- Build files have been written to: /usr/src/concurrent-queue/example/benchmark/build

Scanning dependencies of target bmark-taxicab
[ 25%] Building CXX object CMakeFiles/bmark-taxicab.dir/src/benchmark.cpp.o
[ 50%] Building CXX object CMakeFiles/bmark-taxicab.dir/usr/src/concurrent-queue/example/src/taxicab_number.cpp.o
[ 75%] Building CXX object CMakeFiles/bmark-taxicab.dir/usr/src/concurrent-queue/example/src/utility.cpp.o
[100%] Linking CXX executable ../bmark-taxicab
[100%] Built target bmark-taxicab

++ created directory: output

++ successfully built:
bmark-taxicab - 1319032 bytes

++ pow N : 5
++ pow R : 2
++ size T: 1

--------------------------------------------------------------------------------
./bmark-taxicab
2021-01-27T16:22:51+03:00
Running ./bmark-taxicab
Run on (4 X 3900 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 4096 KiB (x1)
Load Average: 1.81, 1.89, 1.91
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
***WARNING*** Library was built as DEBUG. Timings may be affected.
output/str_2021-01-27_16-22-52.json
output/str_2021-01-27_16-22-53.json
output/str_2021-01-27_16-22-54.json
output/str_2021-01-27_16-22-56.json
output/str_2021-01-27_16-22-57.json
output/str_2021-01-27_16-22-58.json
output/str_2021-01-27_16-22-59.json
output/str_2021-01-27_16-23-00.json
output/str_2021-01-27_16-23-01.json
output/str_2021-01-27_16-23-02.json
output/str_2021-01-27_16-23-03.json
-------------------------------------------------------------------
Benchmark                         Time             CPU   Iterations
-------------------------------------------------------------------
BenchmarkTaxiCabStr/inst 1107474963 ns       916512 ns           10
output/int_2021-01-27_16-23-04.json
output/int_2021-01-27_16-23-05.json
output/int_2021-01-27_16-23-07.json
output/int_2021-01-27_16-23-08.json
output/int_2021-01-27_16-23-09.json
output/int_2021-01-27_16-23-10.json
output/int_2021-01-27_16-23-11.json
output/int_2021-01-27_16-23-12.json
output/int_2021-01-27_16-23-13.json
output/int_2021-01-27_16-23-14.json
output/int_2021-01-27_16-23-15.json
BenchmarkTaxiCabInt/inst 1103920832 ns       304907 ns           10
```

If a clean state of the repository is desired, the [git clean](https://git-scm.com/docs/git-clean) command can be used.

```
$ git clean -ndX
Would remove example/benchmark/bmark-taxicab
Would remove example/benchmark/build/
Would remove example/benchmark/output/
Would remove example/build/
Would remove example/output/
Would remove example/taxicab
Would remove example/test/build/
Would remove example/test/output/
Would remove example/test/test-taxicab
Would remove test/build/
Would remove test/test-queue

$ git clean -fdX
Removing example/benchmark/bmark-taxicab
Removing example/benchmark/build/
Removing example/benchmark/output/
Removing example/build/
Removing example/output/
Removing example/taxicab
Removing example/test/build/
Removing example/test/output/
Removing example/test/test-taxicab
Removing test/build/
Removing test/test-queue
```
