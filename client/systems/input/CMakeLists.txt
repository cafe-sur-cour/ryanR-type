find_package(asio CONFIG REQUIRED)

add_library(MovementInputSystem SHARED MovementInputSystem.cpp MovementInputSystem.hpp)
set_target_properties(MovementInputSystem PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/systems)
target_include_directories(MovementInputSystem PUBLIC ${CMAKE_SOURCE_DIR}/common ${CMAKE_SOURCE_DIR}/client ${CMAKE_SOURCE_DIR}/libs)
target_link_libraries(MovementInputSystem ${ECS_LIB_NAME} asio::asio InputNormalizer ClientNetworkLib)

add_library(ShootInputSystem SHARED ShootInputSystem.cpp ShootInputSystem.hpp)
set_target_properties(ShootInputSystem PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/systems)
target_include_directories(ShootInputSystem PUBLIC ${CMAKE_SOURCE_DIR}/common ${CMAKE_SOURCE_DIR}/client ${CMAKE_SOURCE_DIR}/libs)
target_link_libraries(ShootInputSystem ${ECS_LIB_NAME} asio::asio ClientNetworkLib)
