find_package(SFML COMPONENTS Graphics Window System Audio REQUIRED)

target_sources(${MULTIMEDIA_LIB_NAME} PUBLIC
    SfmlWindow.cpp
    SfmlWindow.hpp
    SfmlEvent.cpp
    SfmlEvent.hpp
    SfmlAudio.cpp
    SfmlAudio.hpp
    SfmlKeyMappings.cpp
    SfmlKeyMappings.hpp
    SfmlEntry.cpp
    TextureManager.cpp
    TextureManager.hpp
    FontManager.cpp
    FontManager.hpp
    ShaderManager.cpp
    ShaderManager.hpp
    RoundedRectangleShape.cpp
    RoundedRectangleShape.hpp
)

target_link_libraries(${MULTIMEDIA_LIB_NAME}
    PUBLIC SFML::Graphics SFML::Window SFML::System SFML::Audio
)
