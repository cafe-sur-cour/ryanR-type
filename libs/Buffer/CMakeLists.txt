set(BUFFER_LIB_NAME Buffer)

add_library(${BUFFER_LIB_NAME} SHARED
    CircularBuffer.hpp
    CircularBuffer.cpp
)

if(WIN32)
    set_target_properties(${BUFFER_LIB_NAME} PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS ON
    )
endif()

target_include_directories(${BUFFER_LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(USE_UNITY_BUILD)
    set_target_properties(${BUFFER_LIB_NAME} PROPERTIES UNITY_BUILD ON)
endif()

add_custom_command(TARGET ${BUFFER_LIB_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${BUFFER_LIB_NAME}> ${CMAKE_SOURCE_DIR}/libraries/
)
