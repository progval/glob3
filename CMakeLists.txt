cmake_minimum_required(VERSION 2.6)
project(glob3)
add_definitions(-std=c99)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

##############################
# Options

set(build_gui 1)
set(build_net 0)
set(build_server 0)
set(build_client 0)

if ((NOT build_gui) AND (NOT build_server))

endif ((NOT build_gui) AND (NOT build_server))

set(PROJECT_ROOT_DIR "${PROJECT_SOURCE_DIR}")
set(PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
set(resources_prefix "${CMAKE_INSTALL_PREFIX}/share")

configure_file (
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
    )
include_directories("${PROJECT_BINARY_DIR}")

##############################
# Global libraries

find_package (SDL REQUIRED)
include_directories(${SDL_INCLUDE_DIRS})
set(LIBS ${LIBS} ${LIBSDL_LIBRARIES})


##############################
# Local libraries

add_library(log src/log.c)

add_library(player src/player.c)

add_library(building src/building.c)
target_link_libraries(player building)

add_library(unit src/unit.c)
target_link_libraries(unit player building)

add_library(map src/map.c)
target_link_libraries(map unit building)

add_library(game src/game.c)
target_link_libraries(game map player unit building)

add_library(gui src/gui.c)
target_link_libraries(gui ${SDL_LIBRARY})


##############################
# Executable

add_executable(glob3 src/main.c)
target_link_libraries(glob3 log game map gui)

##############################
# Install

install(TARGETS glob3 DESTINATION bin)
if(build_gui)
    install(DIRECTORY ${PROJECT_ROOT_DIR}/resources/graphics DESTINATION share)
endif(build_gui)
