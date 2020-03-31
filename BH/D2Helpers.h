#pragma once
#include <Windows.h>
#include "D2Structs.h"

#define INVALID_PARTY_ID 0xFFFF
enum Dll { D2CLIENT = 0, D2COMMON, D2GFX, D2LANG, D2WIN, D2NET, D2GAME, D2LAUNCH, FOG, BNCLIENT, STORM, D2CMP, D2MULTI, D2MCPCLIENT };

bool IsValidMonster(UnitAny *pUnit);
int GetRelation(UnitAny* unit);
RosterUnit* FindPlayerRoster(DWORD unitId);
int GetDllOffset(Dll dll, int offset);