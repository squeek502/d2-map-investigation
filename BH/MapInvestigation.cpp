#include "MapInvestigation.h"

#include <iostream>
#include <fstream>

#include "D2Ptrs.h"
#include "D2Stubs.h"
#include "Common.h"
#include "Constants.h"
#include "BH.h"
#include "DataDispatcher.h"
#include "ArcaneAggregator.h"
#include "PresetsAggregator.h"
#include "Room2Aggregator.h"
#include "LevelDirectionsAggregator.h"

namespace MapInvestigation
{

int actStartingLevelIds[] = { MAP_A1_ROGUE_ENCAMPMENT, MAP_A2_LUT_GHOLEIN, MAP_A3_KURAST_DOCKS, MAP_A4_THE_PANDEMONIUM_FORTRESS, MAP_A5_HARROGATH, MAP_ID_MAX };

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

void InvestigateMaps()
{
	UnitAny* player = D2CLIENT_GetPlayerUnit();
	if (!player || !player->pAct) {
		cout << "No player or act, bailing" << endl;
		return;
	}

	DataDispatcher dispatcher;

	ArcaneAggregator arcaneAggregator;
	dispatcher.add(&arcaneAggregator, "arcane.csv");

	PresetsAggregator presetsAggregator;
	dispatcher.add(&presetsAggregator, "areas.rooms.presets.csv");

	Room2Aggregator room2Aggregator;
	dispatcher.add(&room2Aggregator, "areas.rooms.csv");

	LevelDirectionsAggregator levelDirectionsAggregator;
	dispatcher.add(&levelDirectionsAggregator, "areas.positional.relationships.csv");

	dispatcher.dumpHeaders();

	DWORD playerSeed = D2CLIENT_GetPlayerUnit()->pAct->dwMapSeed;
	//DWORD mapSeed = playerSeed;
	for (DWORD mapSeed = 0; mapSeed < 100; mapSeed++)
	{
		dispatcher.onNewSeed(mapSeed);

		cout << ">" << mapSeed << endl;
		bool isLOD = *p_D2CLIENT_ExpCharFlag;
		int lastAct = 2; //(isLOD ? 5 : 4);
		for (int act = 1; act <= lastAct; act++)
		{
			// the last two arguments seem to be related to automap updating; by setting them to NULL it stops some automap artifacts
			// and gets rid of some memory-leak-like behavior (before D2CLIENT_LoadAct_1, D2CLIENT_LoadAct_2 were passed to these args)
			Act* pAct = D2COMMON_LoadAct(act - 1, mapSeed, isLOD, 0, D2CLIENT_GetDifficulty(), NULL, actStartingLevelIds[act - 1], NULL, NULL);
			if (!pAct || !pAct->pMisc) {
				cout << "Failed to load act, bailing" << endl;
				return;
			}

			for (int level = actStartingLevelIds[act - 1]; level < actStartingLevelIds[act]; level++)
			{
				Level* pLevel = GetLevel(pAct, level);
				if (!pLevel)
					continue;
				if (!pLevel->pRoom2First)
					D2COMMON_InitLevel(pLevel);

				dispatcher.onLevelLoad(pLevel);

				for (Room2* room = pLevel->pRoom2First; room; room = room->pRoom2Next)
				{
					dispatcher.onRoom2Load(room);

					bool roomDataNeedsCleanup = false;

					if (!room->pRoom1) {
						D2COMMON_AddRoomData(pLevel->pMisc->pAct, pLevel->dwLevelNo, room->dwPosX, room->dwPosY, room->pRoom1);
						roomDataNeedsCleanup = true;
					}

					if (!room->pRoom1)
						continue;

					dispatcher.onRoom1Load(room->pRoom1);

					for (PresetUnit* pUnit = room->pPreset; pUnit; pUnit = pUnit->pPresetNext)
					{
						dispatcher.onPresetUnitLoad(room, pUnit);
					}

					if (roomDataNeedsCleanup)
						D2COMMON_RemoveRoomData(pLevel->pMisc->pAct, pLevel->dwLevelNo, room->dwPosX, room->dwPosY, room->pRoom1);
				}
			}

			D2COMMON_UnloadAct(pAct);
		}
		dispatcher.onSeedEnd(mapSeed);
	}
}

}