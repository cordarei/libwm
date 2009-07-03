include_directories(${CMAKE_CURRENT_SOURCE_DIR})

aux_source_directory(common COMMON_SOURCES)

if(WIN32)
	add_definitions(-DUNICODE)

	aux_source_directory(win32 WIN32_SOURCES)
	add_library(wm ${COMMON_SOURCES} ${WIN32_SOURCES})
	target_link_libraries(wm ${X11_LIBRARIES})
	target_link_libraries(wm ${OPENGL_gl_LIBRARY})
endif(WIN32)

if(UNIX)
	find_package(X11 REQUIRED)
	include_directories(${X11_INCLUDE_DIR})

	if(APPLE)
		find_path(GLX_INCLUDE_DIR GL/glx.h)
		if(NOT GLX_INCLUDE_DIR)
			message(FATAL_ERROR "Can't locate glx.h")
		endif(NOT GLX_INCLUDE_DIR)

		find_library(GLX_LIBRARY NAMES GLX glx GL gl)
		if(NOT GLX_INCLUDE_DIR)
			message(FATAL_ERROR "Can't locate GLX library")
		endif(NOT GLX_INCLUDE_DIR)

		include_directories(${GLX_INCLUDE_DIR})
	else(APPLE)
		set(GLX_LIBRARY )
	endif(APPLE)
	
	aux_source_directory(xlib XLIB_SOURCES)
	aux_source_directory(glx GLX_SOURCES)
	add_library(wm ${COMMON_SOURCES} ${XLIB_SOURCES} ${GLX_SOURCES})
	target_link_libraries(wm ${X11_LIBRARIES})
	target_link_libraries(wm ${OPENGL_gl_LIBRARY} ${GLX_LIBRARY})
endif(UNIX)

add_subdirectory(test)

set_target_properties(wm PROPERTIES VERSION ${WM_VERSION} SOVERSION ${WM_VERSION_MAJOR})
install(TARGETS wm DESTINATION lib)

