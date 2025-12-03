set(MULTIMEDIA_LIB_NAME Multimedia)

find_package(SFML COMPONENTS Graphics Window System Audio REQUIRED)

# Read the number of embedded asset files
if(EXISTS ${CMAKE_BINARY_DIR}/EmbeddedAssets_count.txt)
    file(READ ${CMAKE_BINARY_DIR}/EmbeddedAssets_count.txt EMBEDDED_ASSETS_COUNT)
    string(STRIP "${EMBEDDED_ASSETS_COUNT}" EMBEDDED_ASSETS_COUNT)
    
    # Generate list of embedded asset files
    set(EMBEDDED_ASSET_FILES "")
    math(EXPR EMBEDDED_ASSETS_MAX "${EMBEDDED_ASSETS_COUNT} - 1")
    foreach(i RANGE 0 ${EMBEDDED_ASSETS_MAX})
        list(APPEND EMBEDDED_ASSET_FILES ${CMAKE_BINARY_DIR}/embeddedAssets/EmbeddedAsset_${i}.cpp)
    endforeach()
else()
    set(EMBEDDED_ASSET_FILES "")
endif()

add_library(${MULTIMEDIA_LIB_NAME} SHARED
    IAudio.hpp
    IEvent.hpp
    IWindow.hpp
    SfmlWindow.cpp
    SfmlWindow.hpp
    SfmlEvent.cpp
    SfmlEvent.hpp
    SfmlAudio.cpp
    SfmlAudio.hpp
    SfmlKeyMappings.cpp
    SfmlKeyMappings.hpp
    TextureManager.cpp
    TextureManager.hpp
    FontManager.cpp
    FontManager.hpp
    SfmlEntry.cpp
    EventTypes.hpp
    ${CMAKE_BINARY_DIR}/EmbeddedAssets.cpp
    ${EMBEDDED_ASSET_FILES}
)

set_source_files_properties(${CMAKE_BINARY_DIR}/EmbeddedAssets.cpp PROPERTIES GENERATED TRUE)
if(EMBEDDED_ASSET_FILES)
    set_source_files_properties(
        ${EMBEDDED_ASSET_FILES}
        PROPERTIES GENERATED TRUE
    )
endif()

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
    if(EMBEDDED_ASSET_FILES)
        set_source_files_properties(
            ${CMAKE_BINARY_DIR}/EmbeddedAssets.cpp
            ${EMBEDDED_ASSET_FILES}
            PROPERTIES COMPILE_FLAGS "/Od /bigobj"
        )
    else()
        set_source_files_properties(
            ${CMAKE_BINARY_DIR}/EmbeddedAssets.cpp
            PROPERTIES COMPILE_FLAGS "/Od /bigobj"
        )
    endif()
endif()

target_include_directories(${MULTIMEDIA_LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_SOURCE_DIR}/common
    PUBLIC ${CMAKE_BINARY_DIR}
)

target_link_libraries(${MULTIMEDIA_LIB_NAME}
    PUBLIC SFML::Graphics SFML::Window SFML::System SFML::Audio
    PUBLIC ECS
)

add_custom_command(TARGET ${MULTIMEDIA_LIB_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${MULTIMEDIA_LIB_NAME}> ${CMAKE_SOURCE_DIR}/libraries/
)
