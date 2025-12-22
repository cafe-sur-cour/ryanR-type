add_library(ServerInputLib STATIC
    ServerInputProvider.hpp
    ServerInputProvider.cpp
)

set_target_properties(ServerInputLib PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(ServerInputLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(ServerInputLib PUBLIC
    ERROR
)

target_sources(${SERVER_LIB_NAME} PRIVATE
    initResourcesManager.hpp
    initResourcesManager.cpp
)
