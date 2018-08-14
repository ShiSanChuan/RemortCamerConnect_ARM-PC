cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(camer VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 11)

INCLUDE_DIRECTORIES(include)
add_subdirectory(src)
