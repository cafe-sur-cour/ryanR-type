set(MULTIMEDIA_LIB_NAME Multimedia)

find_package(SFML COMPONENTS Graphics Window System Audio REQUIRED)

add_library(${MULTIMEDIA_LIB_NAME} SHARED
    SfmlWindow.cpp
    SfmlEvent.cpp
    SfmlKeyMappings.cpp
    TextureManager.cpp
    FontManager.cpp
    MultimediaEntry.cpp
    ${CMAKE_BINARY_DIR}/EmbeddedAssets.cpp
    ${CMAKE_BINARY_DIR}/embeddedAssets/EmbeddedAsset_0.cpp
    ${CMAKE_BINARY_DIR}/embeddedAssets/EmbeddedAsset_1.cpp
    ${CMAKE_BINARY_DIR}/embeddedAssets/EmbeddedAsset_2.cpp
    ${CMAKE_SOURCE_DIR}/common/types/FRect.cpp
)

set_source_files_properties(${CMAKE_BINARY_DIR}/EmbeddedAssets.cpp PROPERTIES GENERATED TRUE)
set_source_files_properties(
    ${CMAKE_BINARY_DIR}/embeddedAssets/EmbeddedAsset_0.cpp
    ${CMAKE_BINARY_DIR}/embeddedAssets/EmbeddedAsset_1.cpp
    ${CMAKE_BINARY_DIR}/embeddedAssets/EmbeddedAsset_2.cpp
    PROPERTIES GENERATED TRUE
)

if(WIN32)
    set_target_properties(${MULTIMEDIA_LIB_NAME} PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS ON
    )
endif()

add_subdirectory(AssetManager)

if(TARGET generate_embedded_assets)
    add_dependencies(${MULTIMEDIA_LIB_NAME} generate_embedded_assets)
endif()

if(MSVC)
    set_source_files_properties(
        ${CMAKE_BINARY_DIR}/EmbeddedAssets.cpp
        ${CMAKE_BINARY_DIR}/embeddedAssets/EmbeddedAsset_0.cpp
        ${CMAKE_BINARY_DIR}/embeddedAssets/EmbeddedAsset_1.cpp
        ${CMAKE_BINARY_DIR}/embeddedAssets/EmbeddedAsset_2.cpp
        PROPERTIES COMPILE_FLAGS "/Od /bigobj"
    )
endif()

target_include_directories(${MULTIMEDIA_LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_SOURCE_DIR}/common
    PUBLIC ${CMAKE_BINARY_DIR}
)

target_link_libraries(${MULTIMEDIA_LIB_NAME}
    PUBLIC SFML::Graphics SFML::Window SFML::System SFML::Audio
)

add_custom_command(TARGET ${MULTIMEDIA_LIB_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${MULTIMEDIA_LIB_NAME}> ${CMAKE_SOURCE_DIR}/libraries/
)
