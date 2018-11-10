cmake_minimum_required(VERSION 2.8)
project(hardware-effects)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(branch-misprediction)
add_subdirectory(cache-aliasing)
add_subdirectory(false-sharing)
add_subdirectory(write-combining)
