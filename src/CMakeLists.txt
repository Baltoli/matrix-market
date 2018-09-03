cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(matrix-market VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
  -Wall -pedantic \
  -Wno-sign-compare")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} \
  -O3")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} \
  -g -O0")

include(GNUInstallDirs)

enable_testing()

add_subdirectory(bench)
add_subdirectory(mm)
