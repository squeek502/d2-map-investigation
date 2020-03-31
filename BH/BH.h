#pragma once
#include <string>
#include <Windows.h>

using namespace std;

struct cGuardModule
{	
	union {
		HMODULE hModule;
		DWORD dwBaseAddress;
	};
	DWORD _1;
	char szPath[MAX_PATH];
};

namespace BH {
	extern string path;
	extern HINSTANCE instance;
	extern bool cGuardLoaded;

	extern bool Startup(HINSTANCE instance, VOID* reserved);
	extern bool Shutdown();
};