cmake_minimum_required(VERSION 3.13)

project(box2d-tiled-platformer-demo)

find_package(sdl2 REQUIRED)
find_package(box2d REQUIRED)

include(FindPkgConfig)
pkg_search_module(SDL2_gfx REQUIRED SDL2_gfx)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

include_directories(
    ${SDL2_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}
    src/PhysicsDebugDrawer.cpp
    src/PhysicsUtilities.cpp
    src/GameObject.cpp
    src/PhysicsContactListener.cpp
    src/Level.cpp
    src/Player.cpp
    src/Game.cpp
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
    ${SDL2_LIBRARIES}
    ${SDL2_gfx_LIBRARIES}
    box2d::box2d
)
