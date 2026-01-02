set(ASIO_LIB_NAME AsioNetwork)

# Fichiers sources pour la bibliothèque Asio
set(ASIO_SOURCES
    Address.cpp
    Endpoint.cpp
    ErrorCode.cpp
    Resolver.cpp
    Socket.cpp
)

# Fichiers d'en-tête
set(ASIO_HEADERS
    Address.hpp
    Endpoint.hpp
    ErrorCode.hpp
    Resolver.hpp
    Socket.hpp
)

# Création de la bibliothèque statique
add_library(${ASIO_LIB_NAME} STATIC
    ${ASIO_SOURCES}
    ${ASIO_HEADERS}
)

# Trouver le package Asio
find_package(asio CONFIG REQUIRED)

# Lier la bibliothèque avec Asio
target_link_libraries(${ASIO_LIB_NAME}
    PUBLIC
        asio::asio
)

# Inclure les répertoires nécessaires
target_include_directories(${ASIO_LIB_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Définir des macros pour Asio si nécessaire
target_compile_definitions(${ASIO_LIB_NAME}
    PUBLIC
        ASIO_STANDALONE
)

# Exporter le nom de la bibliothèque pour les autres parties du projet
set(ASIO_LIB_NAME ${ASIO_LIB_NAME} PARENT_SCOPE)
