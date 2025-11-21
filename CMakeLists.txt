cmake_minimum_required(VERSION 3.23)
project(r-type VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(libs)
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(tests)
