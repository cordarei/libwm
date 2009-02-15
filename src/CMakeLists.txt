if(WIN32)
	find_package(OpenGL REQUIRED)
	include_directories(${OPENGL_INCLUDE_DIR})
	
	add_library(wm win32/display.cpp win32/window.cpp win32/context.cpp win32/error.cpp)
	target_link_libraries(wm ${X11_LIBRARIES})
	target_link_libraries(wm ${OPENGL_gl_LIBRARY})
endif(WIN32)


if(UNIX)
	find_package(X11 REQUIRED)
	include_directories(${X11_INCLUDE_DIR})

	find_package(OpenGL REQUIRED)
	include_directories(${OPENGL_INCLUDE_DIR})
	
	add_library(wm xlib/display.cpp xlib/window.cpp xlib/context.cpp)
	target_link_libraries(wm ${X11_LIBRARIES})
	target_link_libraries(wm ${OPENGL_gl_LIBRARY})
endif(UNIX)


