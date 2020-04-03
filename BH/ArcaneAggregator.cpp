#include "ArcaneAggregator.h"
#include <iostream>

using namespace std;

#define ROOM_ARCANE_END_BOTTOM_RIGHT 525
#define ROOM_ARCANE_END_TOP_LEFT 526
#define ROOM_ARCANE_END_TOP_RIGHT 527
#define ROOM_ARCANE_END_BOTTOM_LEFT 528

#define PRESET_ARCANE_TELEPORT_PAD1 192

namespace MapInvestigation
{

void ArcaneAggregator::onNewSeed(uint32_t seed)
{
	DataHandler::onNewSeed(seed);
	arcaneDirection = Direction::Unknown;
	portalsDirection = Direction::Unknown;
}

void ArcaneAggregator::onSeedEnd(uint32_t seed)
{
	this->dumpData(*this->stream);
}

void ArcaneAggregator::onRoom2Load(Room2 * room)
{
	if (room->pType2Info->dwRoomNumber >= ROOM_ARCANE_END_BOTTOM_RIGHT && room->pType2Info->dwRoomNumber <= ROOM_ARCANE_END_BOTTOM_LEFT)
	{
		switch (room->pType2Info->dwRoomNumber)
		{
		case ROOM_ARCANE_END_BOTTOM_RIGHT:
			arcaneDirection = Direction::BottomRight;
			break;
		case ROOM_ARCANE_END_TOP_LEFT:
			arcaneDirection = Direction::TopLeft;
			break;
		case ROOM_ARCANE_END_TOP_RIGHT:
			arcaneDirection = Direction::TopRight;
			break;
		case ROOM_ARCANE_END_BOTTOM_LEFT:
			arcaneDirection = Direction::BottomLeft;
			break;
		}
	}
}

void ArcaneAggregator::onPresetUnitLoad(Room2* room, PresetUnit* presetUnit)
{
	if (presetUnit->dwTxtFileNo == PRESET_ARCANE_TELEPORT_PAD1)
	{
		D2Coords levelStart(room->pLevel->pRoom2First->dwPosX, room->pLevel->pRoom2First->dwPosY);
		D2Coords roomCoords(room->dwPosX, room->dwPosY);
		Direction dir = roomCoords.directionFrom(levelStart);
		if (portalsDirection != Direction::Unknown && portalsDirection != dir)
		{
			cout << "Mismatched inferred portal direction in Arcane Sanctuary for seed " << seed << endl;
		}
		portalsDirection = dir;
	}
}

void ArcaneAggregator::dumpData(std::ostream & stream)
{
	stream << seed << "," << DirectionString(arcaneDirection) << "," << DirectionString(portalsDirection) << endl;
}

void ArcaneAggregator::dumpHeader(std::ostream& stream)
{
	stream << "seed" << "," << "summoner_dir" << "," << "portals_dir" << endl;
}

}