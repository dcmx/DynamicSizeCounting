cmake_minimum_required(VERSION 3.24)
project(population_protocols_cpp)

set(CMAKE_CXX_STANDARD 17)


# Set optimization level
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# Enable CPU-specific optimizations
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

# Enable link-time optimizations
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")


add_executable(population_protocols_cpp main.cpp)
