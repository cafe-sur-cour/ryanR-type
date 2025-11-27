set(BUFFER_LIB_NAME Buffer)

add_library(${BUFFER_LIB_NAME} SHARED
    IBuffer.hpp
    CircularBuffer.hpp
    CircularBuffer.cpp
)

target_include_directories(${BUFFER_LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_command(TARGET ${BUFFER_LIB_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${BUFFER_LIB_NAME}> ${CMAKE_SOURCE_DIR}/libraries/
)
