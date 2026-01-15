find_package(GTest REQUIRED)

if(BUILD_COVERAGE)
    add_compile_options(-g -O0 --coverage -fno-inline)
    add_link_options(--coverage)
endif()

add_subdirectory(server)
add_subdirectory(common)
