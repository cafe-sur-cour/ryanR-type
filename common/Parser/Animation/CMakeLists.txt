set(ANIMATION_LIB ANIMATION)

set(ANIMATION_SOURCES
    AnimationConditionFactory.cpp
    AnimationConditionsRegistry.cpp
)

set(ANIMATION_HEADERS
    AnimationConditionFactory.hpp
)

add_library(${ANIMATION_LIB} STATIC ${ANIMATION_SOURCES} ${ANIMATION_HEADERS})

target_include_directories(${ANIMATION_LIB} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(${ANIMATION_LIB} PUBLIC
    ECS
)
