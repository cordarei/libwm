cmake_minimum_required(VERSION 2.6)
project(wm)

set(WM_VERSION_MAJOR 0)
set(WM_VERSION_MINOR 0)
set(WM_VERSION_PATCH 1)
set(WM_VERSION "${WM_VERSION_MAJOR}.${WM_VERSION_MINOR}.${WM_VERSION_PATCH}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/wm DESTINATION include) 

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

add_subdirectory(src)
add_subdirectory(test)

set(CPACK_PACKAGE_VERSION_MAJOR ${WM_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${WM_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${WM_VERSION_PATCH})
set(CPACK_GENERATOR TBZ2)
set(CPACK_SOURCE_GENERATOR TBZ2)
set(CPACK_SOURCE_IGNORE_FILES "build;.hg")
include(CPack)

