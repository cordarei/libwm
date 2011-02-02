set(EXECUTABLE_OPTIONS )
set(ENTRYPOINT_LIBRARY wm-entrypoint)

if(WIN32)
	option(BUILD_CONSOLE_APP "Build Console application" NO)

	if(BUILD_CONSOLE_APP)
		set(ENTRYPOINT_LIBRARY wm-entrypoint-console)
	else(BUILD_CONSOLE_APP)
		set(EXECUTABLE_OPTIONS WIN32)
	endif(BUILD_CONSOLE_APP)
endif(WIN32)

add_executable(test ${EXECUTABLE_OPTIONS} main.cpp draw.cpp)
target_link_libraries(test wm ${ENTRYPOINT_LIBRARY})
target_link_libraries(test ${OPENGL_LIBRARIES})

add_executable(vsync ${EXECUTABLE_OPTIONS} vsync.cpp)
target_link_libraries(vsync wm ${ENTRYPOINT_LIBRARY})
target_link_libraries(vsync ${OPENGL_LIBRARIES})
