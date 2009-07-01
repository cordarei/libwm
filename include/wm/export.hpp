#ifndef WM_EXPORT
	#ifdef WIN32
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
