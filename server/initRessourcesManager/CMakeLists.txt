set(SERVER_INIT_LIB ServerInit)

add_library(${SERVER_INIT_LIB} STATIC
    initRessourcesManager.hpp
    initRessourcesManager.cpp
)

target_include_directories(${SERVER_INIT_LIB}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
