add_library(${ERROR_LIB} STATIC)

target_include_directories(${ERROR_LIB}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_sources(${ERROR_LIB} PUBLIC
    IError.hpp
    AError.hpp
    AError.cpp
    ServerErrror.cpp
    ServerErrror.hpp
    PacketError.cpp
    PacketError.hpp
    ParserError.cpp
    ParserError.hpp
)
