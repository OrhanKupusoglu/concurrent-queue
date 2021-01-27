cmake_minimum_required(VERSION 3.5)

include("./CMakeVersion.txt")

set(BUILD_NAME taxicab)

project(${BUILD_NAME} VERSION ${BUILD_MAJOR_VER}.${BUILD_MINOR_VER}.${BUILD_PATCH_VER} LANGUAGES CXX)

# save taxicab number's cubes in the bits of an uint64_t, default is save taxicab number's cubes in std::string
#add_definitions(-DSOLUTION_INT)

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

set(SOURCE_FILES "./src/main.cpp"
                 "./src/taxicab_number.cpp"
                 "./src/utility.cpp")

add_executable(${BUILD_NAME} ${SOURCE_FILES})
set_target_properties(${BUILD_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../.)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${BUILD_NAME} PRIVATE Threads::Threads)
