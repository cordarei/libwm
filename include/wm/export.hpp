#include <wm/config.hpp>

#ifndef WM_EXPORT
	#if defined(WIN32) && defined(WM_BUILD_SHARED_LIBS)
		#ifdef wm_EXPORTS
			// wm_EXPORTS is defined by CMake when building DLL's
			#define WM_EXPORT __declspec(dllexport)
		#else
			#define WM_EXPORT __declspec(dllimport)
		#endif
	#else
		#define WM_EXPORT
	#endif
#endif
