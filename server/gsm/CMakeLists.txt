set(SERVER_GSM_LIB ServerGSM)

add_library(${SERVER_GSM_LIB} STATIC)

target_include_directories(${SERVER_GSM_LIB}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(machine)
add_subdirectory(states)
