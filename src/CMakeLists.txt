include_directories(${CMAKE_CURRENT_SOURCE_DIR})

aux_source_directory(common COMMON_SOURCES)

if(WIN32)
	add_definitions(-DUNICODE)
	add_definitions(-DWIN32_LEAN_AND_MEAN)

	aux_source_directory(win32 WIN32_SOURCES)
	aux_source_directory(wgl WGL_SOURCES)
	add_library(wm ${COMMON_SOURCES} ${WIN32_SOURCES} ${WGL_SOURCES})
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
		mark_as_advanced(GLX_INCLUDE_DIR)

		find_library(GLX_LIBRARY NAMES GLX glx GL gl)
		if(NOT GLX_INCLUDE_DIR)
			message(FATAL_ERROR "Can't locate GLX library")
		endif(NOT GLX_INCLUDE_DIR)
		mark_as_advanced(GLX_LIBRARY)

		include_directories(${GLX_INCLUDE_DIR})
	else(APPLE)
		set(GLX_LIBRARY )
	endif(APPLE)
	
	if(NOT "${CMAKE_SYSTEM_NAME}" MATCHES "BSD$")
		find_library(DLOPEN_LIBRARY NAMES dl)
		if(NOT DLOPEN_LIBRARY)
			message(FATAL_ERROR "-ldl not found")
		endif(NOT DLOPEN_LIBRARY)
	else(NOT "${CMAKE_SYSTEM_NAME}" MATCHES "BSD$")
		set(DLOPEN_LIBRARY "")
	endif(NOT "${CMAKE_SYSTEM_NAME}" MATCHES "BSD$")
	
	find_path(DLOPEN_INCLUDE_DIR dlfcn.h)
	if(NOT DLOPEN_INCLUDE_DIR)
		message(FATAL_ERROR "dlfcn.h not found")
	endif(NOT DLOPEN_INCLUDE_DIR)
	mark_as_advanced(DLOPEN_LIBRARY)
	mark_as_advanced(DLOPEN_INCLUDE_DIR)
	
	include_directories(${DLOPEN_INCLUDE_DIR})

	aux_source_directory(xlib XLIB_SOURCES)
	aux_source_directory(glx GLX_SOURCES)
	add_library(wm ${COMMON_SOURCES} ${XLIB_SOURCES} ${GLX_SOURCES})
	target_link_libraries(wm ${X11_LIBRARIES})
	target_link_libraries(wm ${OPENGL_gl_LIBRARY} ${GLX_LIBRARY})
	target_link_libraries(wm ${DLOPEN_LIBRARY})
endif(UNIX)

set_target_properties(wm PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})
install(TARGETS wm RUNTIME DESTINATION bin LIBRARY DESTINATION lib NAMELINK_SKIP ARCHIVE DESTINATION lib COMPONENT runtime)
install(TARGETS wm RUNTIME DESTINATION bin LIBRARY DESTINATION lib NAMELINK_ONLY ARCHIVE DESTINATION lib COMPONENT devel)

