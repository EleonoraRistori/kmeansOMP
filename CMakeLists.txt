cmake_minimum_required(VERSION 3.22)
project(kmeansOMP)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
# the following 2 lines are needed on macOS.
# update paths as required
include_directories(/usr/local/include/)
link_directories(/usr/local/lib/)

add_executable(kmeansOMP main.cpp s_kmeans.h p_kmeans.h utils.h)
