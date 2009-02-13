cmake_minimum_required(VERSION 2.6)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(Boost REQUIRED)

add_subdirectory(src)
add_subdirectory(test EXCLUDE_FROM_ALL)

