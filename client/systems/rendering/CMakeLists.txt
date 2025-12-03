target_sources(${CLIENT_BIN_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/SpriteRenderingSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SpriteRenderingSystem.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AnimationRenderingSystem.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AnimationRenderingSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/HitboxRenderingSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/HitboxRenderingSystem.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RectangleRenderingSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RectangleRenderingSystem.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TextRenderingSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TextRenderingSystem.hpp
)
