set(ASIO_LIB_NAME AsioNetwork)

set(ASIO_SOURCES
    AsioAddress.cpp
    AsioEndpoint.cpp
    AsioErrorCode.cpp
    AsioResolver.cpp
    AsioSocket.cpp
)

set(ASIO_HEADERS
    AsioAddress.hpp
    AsioEndpoint.hpp
    AsioErrorCode.hpp
    AsioResolver.hpp
    AsioSocket.hpp
)

add_library(${ASIO_LIB_NAME} STATIC
    ${ASIO_SOURCES}
    ${ASIO_HEADERS}
)

find_package(asio CONFIG REQUIRED)

target_link_libraries(${ASIO_LIB_NAME}
    PUBLIC
        asio::asio
)

target_include_directories(${ASIO_LIB_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_compile_definitions(${ASIO_LIB_NAME}
    PUBLIC
        ASIO_STANDALONE
)

set(ASIO_LIB_NAME ${ASIO_LIB_NAME} PARENT_SCOPE)
