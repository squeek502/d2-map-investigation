#include "MapInvestigation.h"

#include <iostream>
#include <fstream>

#include "D2Ptrs.h"
#include "D2Stubs.h"
#include "Common.h"
#include "Constants.h"
#include "BH.h"

using namespace std;

static int actStartingLevelIds[] = { MAP_A1_ROGUE_ENCAMPMENT, MAP_A2_LUT_GHOLEIN, MAP_A3_KURAST_DOCKS, MAP_A4_THE_PANDEMONIUM_FORTRESS, MAP_A5_HARROGATH, MAP_ID_MAX };

static std::ofstream csv;
static std::ofstream arcane_csv;

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

void DumpPresets(Level* level)
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
			csv << level->pMisc->pAct->dwMapSeed << ",";
			char *levelName = UnicodeToAnsi(D2CLIENT_GetLevelName(level->dwLevelNo));
			csv << levelName << " ";
			csv << "Preset ";
			switch (pUnit->dwType)
			{
			case UNIT_PLAYER:
				csv << "Player";
				break;
			case UNIT_MONSTER:
				csv << "Monster";
				break;
			case UNIT_OBJECT:
				csv << "Object";
				break;
			case UNIT_MISSILE:
				csv << "Missle";
				break;
			case UNIT_ITEM:
				csv << "Missle";
				break;
			case UNIT_TILE:
				csv << "Tile";
				break;
			case EXIT:
				csv << "Exit";
				break;
			case XY:
				csv << "XY";
				break;
			}
			if (pUnit->dwType == UNIT_OBJECT && pUnit->dwTxtFileNo <= 572) {
				ObjectTxt* objTxt = D2COMMON_GetObjectTxt(pUnit->dwTxtFileNo);
				csv << " " << pUnit->dwTxtFileNo;
				if (objTxt && objTxt->szName) {
					csv << " " << objTxt->szName;
				}
				if (objTxt && objTxt->wszName) {
					csv << " (" << UnicodeToAnsi(objTxt->wszName) << ")";
				}
				csv << ",";
			}
			else
			{
				csv << " " << pUnit->dwTxtFileNo << ",";
			}
			int deltaX = room->dwPosX - level->pRoom2First->dwPosX;
			int deltaY = room->dwPosY - level->pRoom2First->dwPosY;
			csv << deltaX << "," << deltaY << ",";
			csv << pUnit->dwPosX << "," << pUnit->dwPosY;
			csv << endl;
		}

		//Remove Data if Added
		if (roomData)
			D2COMMON_RemoveRoomData(level->pMisc->pAct, level->dwLevelNo, room->dwPosX, room->dwPosY, room->pRoom1);
	}
}

MapInvestigation::SummonerPosition GetSummonerPositionFromRoom(Room2* room)
{
	int deltaX = room->dwPosX - room->pLevel->pRoom2First->dwPosX;
	int deltaY = room->dwPosY - room->pLevel->pRoom2First->dwPosY;
	if (deltaX == 84) return MapInvestigation::SummonerPosition::BottomRight;
	if (deltaX == -84) return MapInvestigation::SummonerPosition::TopLeft;
	if (deltaY == 84) return MapInvestigation::SummonerPosition::BottomLeft;
	if (deltaY == -84) return MapInvestigation::SummonerPosition::TopRight;
	return MapInvestigation::SummonerPosition::Unknown;
}

bool IsSummonerPreset(PresetUnit* pUnit)
{
	// 250 is the Summoner's hcIdx in MonStats.txt
	return pUnit && pUnit->dwType == UNIT_MONSTER && pUnit->dwTxtFileNo == 250;
}

// TODO: Don't duplicate the work of loading the rooms here, should be merged with DumpPresets in some way
MapInvestigation::SummonerPosition GetSummonerPosition(Level* level)
{
	if (!level->pRoom2First)
		D2COMMON_InitLevel(level);

	MapInvestigation::SummonerPosition pos = MapInvestigation::SummonerPosition::Unknown;

	for (Room2* room = level->pRoom2First; room && pos == MapInvestigation::SummonerPosition::Unknown; room = room->pRoom2Next)
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
			if (IsSummonerPreset(pUnit))
			{
				pos = GetSummonerPositionFromRoom(room);
			}
		}

		//Remove Data if Added
		if (roomData)
			D2COMMON_RemoveRoomData(level->pMisc->pAct, level->dwLevelNo, room->dwPosX, room->dwPosY, room->pRoom1);
	}

	return pos;
}

void DumpMapSeedData(DWORD mapSeed)
{
	bool isLOD = *p_D2CLIENT_ExpCharFlag;
	//int lastAct = (isLOD ? 5 : 4);
	// we only care about acts 1 and 2 (for now)
	for (int act = 1; act <= 2; act++)
	{
		UnitAny* player = D2CLIENT_GetPlayerUnit();
		if (!player || !player->pAct) {
			cout << "No player or act, bailing" << endl;
			return;
		}

		Act* pAct = D2COMMON_LoadAct(act - 1, mapSeed, isLOD, 0, D2CLIENT_GetDifficulty(), NULL, actStartingLevelIds[act - 1], D2CLIENT_LoadAct_1, D2CLIENT_LoadAct_2);
		if (!pAct || !pAct->pMisc) {
			cout << "Failed to load act, bailing" << endl;
			return;
		}

		// Every level
		for (int level = actStartingLevelIds[act - 1]; level < actStartingLevelIds[act]; level++)
		{
			Level* pLevel = GetLevel(pAct, level);
			if (!pLevel) continue;

			DumpPresets(pLevel);
		}

		if (act == 2)
		{
			arcane_csv << mapSeed << ",";
			Level* pLevel = GetLevel(pAct, MAP_A2_ARCANE_SANCTUARY);
			cout << " \t" << "Summoner: ";
			MapInvestigation::SummonerPosition summonerPosition = GetSummonerPosition(pLevel);
			switch (summonerPosition)
			{
			case MapInvestigation::SummonerPosition::TopRight:
				cout << "Top Right";
				arcane_csv << "TopRight";
				break;
			case MapInvestigation::SummonerPosition::BottomRight:
				cout << "Bottom Right";
				arcane_csv << "BottomRight";
				break;
			case MapInvestigation::SummonerPosition::BottomLeft:
				cout << "Bottom Left";
				arcane_csv << "BottomLeft";
				break;
			case MapInvestigation::SummonerPosition::TopLeft:
				cout << "Top Left";
				arcane_csv << "TopLeft";
				break;
			default:
				cout << "Unknown";
				arcane_csv << "Unknown";
				break;
			}
			arcane_csv << endl;
		}

		D2COMMON_UnloadAct(pAct);
	}
}

void InvestigateMaps()
{
	arcane_csv = std::ofstream(BH::path + "/arcane.csv", ios_base::out | ios_base::trunc);
	if (arcane_csv.fail()) {
		cout << "Failed to open arcane.csv, bailing" << endl;
		return;
	}
	arcane_csv << "seed,arcane_pos" << endl;

	csv = std::ofstream(BH::path + "/data.csv", ios_base::out | ios_base::trunc);
	if (csv.fail()) {
		cout << "Failed to open data.csv, bailing" << endl;
		arcane_csv.close();
		return;
	}
	csv << "seed,id,x,y,x2,y2" << endl;

	for (DWORD mapSeed = 0; mapSeed < 50; mapSeed++)
	{
		cout << "Seed: " << mapSeed;
		DumpMapSeedData(mapSeed);
		cout << endl;
	}
	//cout << endl << "Actual Current Seed: " << D2CLIENT_GetPlayerUnit()->pAct->dwMapSeed << endl;
	//DumpMapSeedData(D2CLIENT_GetPlayerUnit()->pAct->dwMapSeed);

	arcane_csv.close();
	csv.close();
}