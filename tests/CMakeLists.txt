find_package(GTest REQUIRED)

add_compile_options(-g -O0 --coverage -fno-inline)
add_link_options(--coverage)

add_subdirectory(server)
add_subdirectory(common)
