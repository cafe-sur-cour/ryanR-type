target_compile_definitions(${NETWORK_LIB_NAME}
    PUBLIC _WIN32_WINNT=0x0A00
    PUBLIC WIN32_LEAN_AND_MEAN
)

add_library(${NETWORK_LIB_NAME} SHARED
    WindowsNetwork.cpp
)
