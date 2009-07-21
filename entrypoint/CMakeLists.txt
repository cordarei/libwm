if(WIN32)
	add_definitions(-DUNICODE)
	add_library(wm-entrypoint STATIC win32/winmain.cpp)
	add_library(wm-entrypoint-console STATIC main/main.cpp)
	
	install(TARGETS wm-entrypoint-console DESTINATION lib COMPONENT devel)
endif(WIN32)

if(UNIX)
	add_library(wm-entrypoint STATIC main/main.cpp)
endif(UNIX)
install(TARGETS wm-entrypoint DESTINATION lib COMPONENT devel)

