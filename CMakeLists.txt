cmake_minimum_required(VERSION 2.8)
project(APRICOT)
add_executable(apricot apricot.cpp)
include(${APRICOT_SOURCE_DIR}/cmake/FindAPR.cmake)
