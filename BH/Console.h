#pragma once
#ifdef _DEBUG
#include "conio.h"
bool RedirectConsoleIO();
bool ReleaseConsole();
bool CreateNewConsole(int16_t minLength);
#endif