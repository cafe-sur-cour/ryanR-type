add_library(TriggerSystem SHARED
    TriggerSystem.hpp
    TriggerSystem.cpp
)

target_link_libraries(TriggerSystem PUBLIC ${ECS_LIB_NAME})

set_target_properties(TriggerSystem PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/systems
)

add_library(InteractionSystem SHARED
    InteractionSystem.hpp
    InteractionSystem.cpp
)

target_link_libraries(InteractionSystem PUBLIC ${ECS_LIB_NAME})

set_target_properties(InteractionSystem PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/systems
)

target_sources(${ECS_LIB_NAME} PUBLIC
    ActionFactory.hpp
    ActionFactory.cpp
    TagRegistry.hpp
    TagRegistry.cpp
)
