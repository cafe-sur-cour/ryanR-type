add_library(ScoreSystem SHARED
    ScoreSystem.hpp
    ScoreSystem.cpp
)

target_link_libraries(ScoreSystem PUBLIC ${ECS_LIB_NAME})

set_target_properties(ScoreSystem PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/systems
)
