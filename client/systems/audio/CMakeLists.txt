add_library(SoundSystem SHARED SoundSystem.cpp SoundSystem.hpp)
set_target_properties(SoundSystem PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/systems)
target_include_directories(SoundSystem PUBLIC ${CMAKE_SOURCE_DIR}/common ${CMAKE_SOURCE_DIR}/client ${CMAKE_SOURCE_DIR}/libs)
target_link_libraries(SoundSystem ${ECS_LIB_NAME} Multimedia)

add_library(MusicSystem SHARED MusicSystem.cpp MusicSystem.hpp)
set_target_properties(MusicSystem PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/systems)
target_include_directories(MusicSystem PUBLIC ${CMAKE_SOURCE_DIR}/common ${CMAKE_SOURCE_DIR}/client ${CMAKE_SOURCE_DIR}/libs)
target_link_libraries(MusicSystem ${ECS_LIB_NAME} Multimedia)
