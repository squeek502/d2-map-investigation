#include "BH.h"
#include "Console.h"
#include "MapInvestigation.h"

#include <Shlwapi.h>

string BH::path;
HINSTANCE BH::instance;
bool BH::cGuardLoaded;

bool BH::Startup(HINSTANCE instance, VOID* reserved) {
	CreateNewConsole(1024);

	BH::instance = instance;
	if (reserved != NULL) {
		cGuardModule* pModule = (cGuardModule*)reserved;
		if(!pModule)
			return FALSE;
		path.assign(pModule->szPath);
		cGuardLoaded = true;
	} else {
		char szPath[MAX_PATH];
		GetModuleFileName(BH::instance, szPath, MAX_PATH);
		PathRemoveFileSpec(szPath);
		path.assign(szPath);
		path += "\\";
		cGuardLoaded = false;
	}

	MapInvestigation::InvestigateMaps();

	return true;
}

bool BH::Shutdown() {
	ReleaseConsole();
	return true;
}