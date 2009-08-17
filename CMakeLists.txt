cmake_minimum_required(VERSION 2.6)
project(libwm)
set(PROJECT_VERSION "0.1.1")
set(PROJECT_VERSION_MAJOR "0")

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type for make-style systems" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

if(NOT BUILD_SHARED_LIBS)
	option(BUILD_SHARED_LIBS "Build shared libraries" ON)
endif(NOT BUILD_SHARED_LIBS)

configure_file(${CMAKE_SOURCE_DIR}/config.hpp.in ${CMAKE_BINARY_DIR}/include/wm/config.hpp)
install(FILES ${CMAKE_BINARY_DIR}/include/wm/config.hpp DESTINATION include/wm COMPONENT devel)
include_directories(${CMAKE_BINARY_DIR}/include)

include_directories(${CMAKE_SOURCE_DIR}/include)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/wm DESTINATION include COMPONENT devel) 

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

add_subdirectory(src)
add_subdirectory(entrypoint)
add_subdirectory(doc)

option(ENABLE_EXAMPLES "Build example applications" OFF)
if(ENABLE_EXAMPLES)
	add_subdirectory(test)
endif(ENABLE_EXAMPLES)

if(WIN32)
	set(CPACK_GENERATOR "ZIP")
else(WIN32)
	set(CPACK_GENERATOR "TBZ2")
endif(WIN32)

set(CPACK_SOURCE_GENERATOR "TBZ2")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A cross-platform windowing and context management C++ library for OpenGL")
set(CPACK_PACKAGE_CONTACT "http://libwm.googlecode.com/")
set(CPACK_SOURCE_IGNORE_FILES "${CMAKE_BINARY_DIR};.hg")
include(CPack)

