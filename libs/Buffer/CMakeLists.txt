set(BUFFER_LIB_NAME Buffer)

add_library(${BUFFER_LIB_NAME} STATIC
    IBuffer.hpp
    CircularBuffer.hpp
    CircularBuffer.cpp
)

target_include_directories(${BUFFER_LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
