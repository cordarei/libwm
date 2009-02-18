if(WIN32)
	add_library(wm win32/display.cpp win32/window.cpp win32/context.cpp win32/error.cpp)
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
	
	add_library(wm xlib/display.cpp xlib/window.cpp xlib/context.cpp xlib/connection.cpp xlib/dispatcher.cpp)
	target_link_libraries(wm ${X11_LIBRARIES})
	target_link_libraries(wm ${OPENGL_gl_LIBRARY} ${GLX_LIBRARY})
endif(UNIX)


