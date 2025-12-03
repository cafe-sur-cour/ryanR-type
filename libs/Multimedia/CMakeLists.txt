set(MULTIMEDIA_LIB_NAME Multimedia)

find_package(SFML COMPONENTS Graphics Window System Audio REQUIRED)

set(EMBEDDED_ASSET_FILES "")
set(ASSET_COUNT 0)

file(GLOB_RECURSE ALL_ASSETS "${CMAKE_SOURCE_DIR}/assets/*")
list(LENGTH ALL_ASSETS ASSET_COUNT)

message(STATUS "Found ${ASSET_COUNT} assets in ${CMAKE_SOURCE_DIR}/assets")

if(ASSET_COUNT GREATER 0)
    math(EXPR ASSET_COUNT_MINUS_ONE "${ASSET_COUNT} - 1")
    foreach(i RANGE 0 ${ASSET_COUNT_MINUS_ONE})
        list(APPEND EMBEDDED_ASSET_FILES "${CMAKE_BINARY_DIR}/embeddedAssets/EmbeddedAsset_${i}.cpp")
    endforeach()
endif()

set_source_files_properties(${CMAKE_BINARY_DIR}/EmbeddedAssets.cpp PROPERTIES GENERATED TRUE)
foreach(asset_file ${EMBEDDED_ASSET_FILES})
    set_source_files_properties(${asset_file} PROPERTIES GENERATED TRUE)
endforeach()

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
    ${CMAKE_SOURCE_DIR}/common/types/FRect.cpp
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
        ${EMBEDDED_ASSET_FILES}
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
