#-- bp/test/CMakeLists.txt ---------------------------------*- CMake -*-==#
#
#                 Bellport Low Latency Trading Infrastructure.
#
# Copyright MayStreet 2015 - all rights reserved
#
# $Id$
#===----------------------------------------------------------------------====#
cmake_minimum_required(VERSION 2.8)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

project(BPTest)
add_subdirectory(bp)
