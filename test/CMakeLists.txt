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
target_link_libraries(test wm)
target_link_libraries(test ${ENTRYPOINT_LIBRARY})
