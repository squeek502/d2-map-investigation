#include "MapInvestigation.h"

#include <iostream>

#include "D2Ptrs.h"
#include "D2Stubs.h"
#include "Common.h"
#include "Constants.h"

using namespace std;

static int actStartingLevelIds[] = { MAP_A1_ROGUE_ENCAMPMENT, MAP_A2_LUT_GHOLEIN, MAP_A3_KURAST_DOCKS, MAP_A4_THE_PANDEMONIUM_FORTRESS, MAP_A5_HARROGATH, MAP_ID_MAX };

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

MapInvestigation::SummonerPosition GetSummonerPosition(Level* level)
{
	if (!level->pRoom2First)
		D2COMMON_InitLevel(level);

	for (Room2* room = level->pRoom2First; room; room = room->pRoom2Next)
	{
		bool roomData = false;

		//Add Room1 Data if it is not already there.
		if (!room->pRoom1) {
			D2COMMON_AddRoomData(level->pMisc->pAct, level->dwLevelNo, room->dwPosX, room->dwPosY, room->pRoom1);
			roomData = true;
		}

		//Make sure we have Room1
		if (!room->pRoom1)
			continue;

		for (PresetUnit* pUnit = room->pPreset; pUnit; pUnit = pUnit->pPresetNext)
		{
			// not sure exactly what 250 corresponds to, but there is one per Arcane Sanctuary
			if (pUnit->dwType == UNIT_MONSTER && pUnit->dwTxtFileNo == 250)
			{
				int deltaX = room->dwPosX - level->pRoom2First->dwPosX;
				int deltaY = room->dwPosY - level->pRoom2First->dwPosY;
				if (deltaX ==  84) return MapInvestigation::SummonerPosition::BottomRight;
				if (deltaX == -84) return MapInvestigation::SummonerPosition::TopLeft;
				if (deltaY ==  84) return MapInvestigation::SummonerPosition::BottomLeft;
				if (deltaY == -84) return MapInvestigation::SummonerPosition::TopRight;
			}
		}

		//Remove Data if Added
		if (roomData)
			D2COMMON_RemoveRoomData(level->pMisc->pAct, level->dwLevelNo, room->dwPosX, room->dwPosY, room->pRoom1);
	}

	return MapInvestigation::SummonerPosition::Unknown;
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
		Act* pAct = D2COMMON_LoadAct(act - 1, mapSeed, isLOD, 0, D2CLIENT_GetDifficulty(), NULL, actStartingLevelIds[act - 1], D2CLIENT_LoadAct_1, D2CLIENT_LoadAct_2);
		if (!pAct || !pAct->pMisc) {
			cout << "Failed to load act, bailing" << endl;
			return;
		}

		cout << " Act " << act << ": ";

		// To iterate every level for the given act:
		// for (int level = actStartingLevelIds[act - 1]; level < actStartingLevelIds[act]; level++) {

		// for a proof-of-concept, just print location of the second area in each act
		int level = actStartingLevelIds[act - 1] + 2; {
			Level* pLevel = GetLevel(pAct, level);
			if (!pLevel)
				continue;
			if (!pLevel->pRoom2First)
				D2COMMON_InitLevel(pLevel);
			char *levelName = UnicodeToAnsi(D2CLIENT_GetLevelName(pLevel->dwLevelNo));
			cout << levelName << ": " << pLevel->pRoom2First->dwPosX << "," << pLevel->pRoom2First->dwPosY << endl;
		}

		if (act == 2)
		{
			Level* pLevel = GetLevel(pAct, MAP_A2_ARCANE_SANCTUARY);
			cout << "  Summoner: ";
			MapInvestigation::SummonerPosition summonerPosition = GetSummonerPosition(pLevel);
			switch (summonerPosition)
			{
			case MapInvestigation::SummonerPosition::TopRight:
				cout << "Top Right";
				break;
			case MapInvestigation::SummonerPosition::BottomRight:
				cout << "Bottom Right";
				break;
			case MapInvestigation::SummonerPosition::BottomLeft:
				cout << "Bottom Left";
				break;
			case MapInvestigation::SummonerPosition::TopLeft:
				cout << "Top Left";
				break;
			default:
				cout << "Unknown";
				break;
			}
			cout << endl;
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
	cout << endl << "Actual Current Seed: " << D2CLIENT_GetPlayerUnit()->pAct->dwMapSeed << endl;
	DumpMapSeedData(D2CLIENT_GetPlayerUnit()->pAct->dwMapSeed);
}