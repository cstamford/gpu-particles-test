cmake_minimum_required(VERSION 3.13)
project(Borealis VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory(external)
add_subdirectory(src)
