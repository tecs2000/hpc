cmake_minimum_required(VERSION 3.22)

# Project name and version
project(
    Knn 
    VERSION 1.0
    DESCRIPTION "KNN implementation using high performance computing (HPC) techniques."
    HOMEPAGE_URL "https://github.com/tecs2000/hpc/tree/main/KNN"
    LANGUAGES CXX)

# CXX Version
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-O3" "-fopenmp")

# Executable
add_executable(${PROJECT_NAME} knn.cc)

# Add the build directory to use when compiling a given target.
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR})
