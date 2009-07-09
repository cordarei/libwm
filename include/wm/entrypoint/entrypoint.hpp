#ifndef WM_ENTRYPOINT_HPP
#define WM_ENTRYPOINT_HPP

/// Application entry point
/**
	An optional application entry point. Link the application
	with the wm-entrypoint library which contains the
	actual application entry point (main or WinMain) that
	calls the user-defined wm_main function.

	It is not mandatory to use this entry point if the
	application defines the main and/or WinMain functions.

	@param argc number of command line arguments
	@param argv array of command line arguments
	@return application return code
*/
int wm_main(int argc, char* argv[]);

#endif
