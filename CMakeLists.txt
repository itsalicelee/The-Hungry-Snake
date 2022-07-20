cmake_minimum_required(VERSION 3.22)
project(DSAP_Final)

set(CMAKE_CXX_STANDARD 17)

add_executable(DSAP_Final src/main.cpp src/Snake.cpp src/Game.cpp src/Position.cpp src/GUI.cpp)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(DSAP_Final PRIVATE sfml-system sfml-network sfml-graphics sfml-window)
