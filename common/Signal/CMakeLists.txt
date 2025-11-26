set(SIGNAL_LIB SIGNAL)

add_library(${SIGNAL_LIB} STATIC
    Signal.cpp
    Signal.hpp
)

target_include_directories(${SIGNAL_LIB}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
