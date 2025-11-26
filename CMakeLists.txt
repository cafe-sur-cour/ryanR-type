cmake_minimum_required(VERSION 3.23)

project(r-type)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0A00)
endif()

find_package(asio CONFIG REQUIRED)
option(BUILD_TESTS "Build unit tests" OFF)


if(UNIX)
    add_compile_options(-Wall -Wextra -Werror -Wshadow -Wpedantic -Wconversion -Wsign-conversion -Wfloat-equal -Wcast-align -Wformat=2 -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wdouble-promotion -Wuse-after-free -Wno-null-dereference)
elseif(WIN32)
    add_compile_options(/W4 /WX /permissive-)
endif()

enable_testing()

add_subdirectory(common)
add_subdirectory(libs)
add_subdirectory(client)
add_subdirectory(server)


if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
