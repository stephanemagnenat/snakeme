cmake_minimum_required(VERSION 2.8.0)

project (snakeme)

find_package(SDL REQUIRED)
find_package(SDL_mixer REQUIRED)
include_directories(${SDL_INCLUDE_DIR} ${SDL_MIXER_INCLUDE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/snakeme)

add_subdirectory(SGU/src)
add_subdirectory(src)
add_subdirectory(makprev)
