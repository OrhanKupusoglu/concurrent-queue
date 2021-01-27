cmake_minimum_required(VERSION 3.5)

include("../CMakeVersion.txt")

set(BUILD_NAME bmark-taxicab)

project(${BUILD_NAME} VERSION ${BUILD_MAJOR_VER}.${BUILD_MINOR_VER}.${BUILD_PATCH_VER} LANGUAGES CXX)

#add_definitions(-DBMARK_POW_N=5)
#add_definitions(-DBMARK_POW_R=2)
#add_definitions(-DBMARK_SIZE_T=1)
add_definitions(-DBMARK_POW_N=$ENV{BMARK_POW_N})
add_definitions(-DBMARK_POW_R=$ENV{BMARK_POW_R})
add_definitions(-DBMARK_SIZE_T=$ENV{BMARK_SIZE_T})

get_directory_property(DirDefs COMPILE_DEFINITIONS)
message("++ Compile definitions: ${DirDefs}")

#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Release)
message("++ CMake build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD ${BUILD_CPP_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message("++ C++ standard: ${CMAKE_CXX_STANDARD}")

if(CMAKE_BUILD_TYPE STREQUAL Debug )
    message("++ C++ flags: ${CMAKE_CXX_FLAGS_DEBUG}")
else()
    message("++ C++ flags: ${CMAKE_CXX_FLAGS_RELEASE}")
endif()

bitops()

set(SOURCE_FILES "./src/benchmark.cpp"
                 "../src/taxicab_number.cpp"
                 "../src/utility.cpp")

add_executable(${BUILD_NAME} ${SOURCE_FILES})

find_package(benchmark REQUIRED)
target_link_libraries(${BUILD_NAME} benchmark::benchmark)
set_target_properties(${BUILD_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../.)
