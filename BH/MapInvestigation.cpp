#include "MapInvestigation.h"

#include <iostream>

#include "D2Ptrs.h"
#include "D2Stubs.h"
#include "Common.h"

using namespace std;

Level* GetLevel(Act* pAct, int level)
{
	//Insure that the shit we are getting is good.
	if (level < 0 || !pAct)
		return NULL;

	//Loop all the levels in this act

	for (Level* pLevel = pAct->pMisc->pLevelFirst; pLevel; pLevel = pLevel->pNextLevel)
	{
		//Check if we have reached a bad level.
		if (!pLevel)
			break;

		//If we have found the level, return it!
		if (pLevel->dwLevelNo == level && pLevel->dwPosX > 0)
			return pLevel;
	}
	//Default old-way of finding level.
	return D2COMMON_GetLevel(pAct->pMisc, level);
}

void DumpMapSeedData(DWORD mapSeed)
{
	bool isLOD = *p_D2CLIENT_ExpCharFlag;
	for (int act = 1; act <= (isLOD ? 5 : 4); act++)
	{
		UnitAny* player = D2CLIENT_GetPlayerUnit();
		if (!player || !player->pAct) {
			cout << "No player or act, bailing" << endl;
			return;
		}

		// Initalize the act incase it is isn't the act we are in.
		int actIds[6] = { 1, 40, 75, 103, 109, 137 };
		Act* pAct = D2COMMON_LoadAct(act - 1, mapSeed, isLOD, 0, D2CLIENT_GetDifficulty(), NULL, actIds[act - 1], D2CLIENT_LoadAct_1, D2CLIENT_LoadAct_2);
		if (!pAct || !pAct->pMisc) {
			cout << "Failed to load act, bailing" << endl;
			return;
		}

		cout << " Act " << act << ": ";

		// To iterate every level for the given act:
		// for (int level = actIds[act - 1]; level < actIds[act]; level++) {

		// for a proof-of-concept, just print location of the second area in each act
		int level = actIds[act - 1] + 2; {
			Level* pLevel = GetLevel(pAct, level);
			if (!pLevel)
				continue;
			if (!pLevel->pRoom2First)
				D2COMMON_InitLevel(pLevel);
			char *levelName = UnicodeToAnsi(D2CLIENT_GetLevelName(pLevel->dwLevelNo));
			cout << levelName << ": " << pLevel->pRoom2First->dwPosX << "," << pLevel->pRoom2First->dwPosY << endl;
		}

		D2COMMON_UnloadAct(pAct);
	}
}

void InvestigateMaps()
{
	cout << endl << "First tile location for the second area of each act:" << endl;

	// note: the real seed can be found at D2CLIENT_GetPlayerUnit()->pAct->dwMapSeed
	for (DWORD mapSeed = 0; mapSeed < 5; mapSeed++)
	{
		cout << endl << "Seed: " << mapSeed << endl;
		DumpMapSeedData(mapSeed);
	}
}