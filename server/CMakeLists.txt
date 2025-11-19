add_library(server Server.cpp ServerConfig.cpp)
target_include_directories(server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})