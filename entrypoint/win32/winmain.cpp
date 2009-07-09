#include <cstdlib>
#include <vector>

#include <windows.h>

#include <wm/entrypoint/entrypoint.hpp>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR cmdLine, int)
{
	int argc;
	LPWSTR* wargv =
		CommandLineToArgvW(
			GetCommandLineW(),
			&argc);
	if(!wargv)
		return EXIT_FAILURE;

	int size = 0;
	for(int i = 0; i < argc; ++i)
	{
		int len = WideCharToMultiByte(
			CP_UTF8,
			0,
			wargv[i],
			-1,
			0,	// output 
			0,	// size of output buffer
			0,
			0);

		if(!len)
		{
			LocalFree(wargv);
			return EXIT_FAILURE;
		}

		size += len + 1;
	}

	std::vector<char> data(size);
	std::vector<char*> ptrs(argc);
	char *out = &data[0];
	for(int i = 0; i < argc; ++i)
	{
		int len = WideCharToMultiByte(
			CP_UTF8,
			0,
			wargv[i],
			-1,
			out,	// output 
			size,	// size of output buffer
			0,
			0);

		if(!len)
		{
			LocalFree(wargv);
			return EXIT_FAILURE;
		}

		ptrs[i] = out;
		out += len;
		size -= len+1;
		*out++ = 0; // add null terminator
	}

	LocalFree(wargv);
	return wm_main(ptrs.size(), &ptrs[0]);
}
