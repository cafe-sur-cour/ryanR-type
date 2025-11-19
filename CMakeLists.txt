cmake_minimum_required(VERSION 3.10)
project(R-Type)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add subdirectories
add_subdirectory(server)
add_subdirectory(tests)