cmake_minimum_required(VERSION 3.23)

project(r-type)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_subdirectory(common)
add_subdirectory(libs)
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(tests)
