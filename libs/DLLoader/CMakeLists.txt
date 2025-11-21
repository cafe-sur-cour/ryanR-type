set(DLLOADER_LIB_NAME DLLoader)

add_library(${DLLOADER_LIB_NAME} STATIC
    ILoader.hpp
    LoaderType.hpp
    DLLoader.hpp
    DLLoader.cpp
)

target_include_directories(${DLLOADER_LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
