cmake_minimum_required(VERSION 3.16)

project(cephalon VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/)

add_subdirectory(external)
add_subdirectory(cephalon)