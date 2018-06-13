cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(matrix-market VERSION 0.1 LANGUAGES CXX)

include(GNUInstallDirs)

enable_testing()
add_subdirectory(mm)
