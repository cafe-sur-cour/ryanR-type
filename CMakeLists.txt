cmake_minimum_required(VERSION 3.23)

project(r-type)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(asio CONFIG REQUIRED)
add_compile_options(-Wall -Wextra -Werror -Wshadow)

add_subdirectory(common)
add_subdirectory(libs)
add_subdirectory(client)
add_subdirectory(server)
