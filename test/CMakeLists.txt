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

find_package(Boost REQUIRED COMPONENTS date_time thread)

if(Boost_DATE_TIME_FOUND AND Boost_THREAD_FOUND)
	link_directories(${Boost_LIBRARY_DIRS})
	add_executable(application ${EXECUTABLE_OPTIONS} application.cpp)
	target_link_libraries(application wm ${ENTRYPOINT_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_DATE_TIME_LIBRARY})
endif(Boost_DATE_TIME_FOUND AND Boost_THREAD_FOUND)
