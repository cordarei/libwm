cmake_minimum_required(VERSION 2.6)

project(wm)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

add_subdirectory(src)
add_subdirectory(test)

