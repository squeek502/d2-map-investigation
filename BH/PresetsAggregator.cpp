#include "PresetsAggregator.h"
#include "D2Ptrs.h"
#include "D2Stubs.h"
#include "Common.h"
#include "Constants.h"

using namespace std;

namespace MapInvestigation
{

void PresetsAggregator::onPresetUnitLoad(Room2* room, PresetUnit* presetUnit)
{
	levelNo = room->pLevel->dwLevelNo;
	presetType = presetUnit->dwType;
	txtFileNo = presetUnit->dwTxtFileNo;
	absoluteRoomCoords = D2Coords(room->dwPosX, room->dwPosY);
	levelRelativeRoomCoords = absoluteRoomCoords - D2Coords(room->pLevel->pRoom2First->dwPosX, room->pLevel->pRoom2First->dwPosY);
	roomRelativeCoords = D2Coords(presetUnit->dwPosX, presetUnit->dwPosY);
	absoluteCoords = absoluteRoomCoords.toTileCoords() + roomRelativeCoords;
	this->dumpData(*this->stream);
}

void PresetsAggregator::dumpHeader(std::ostream & stream)
{
	stream << "seed";
	stream << ",";
	stream << "level_name";
	stream << ",";
	stream << "id";
	stream << ",";
	stream << "level_relative_room_coords_x,level_relative_room_coords_y";
	stream << ",";
	stream << "absolute_room_coords_x,absolute_room_coords_y";
	stream << ",";
	stream << "room_relative_preset_coords_x,room_relative_preset_coords_y";
	stream << ",";
	stream << "absolute_preset_coords_x,absolute_preset_coords_y";
	stream << endl;
}

void PresetsAggregator::dumpData(std::ostream & stream)
{
	stream << seed << ",";

	char* levelName = UnicodeToAnsi(D2CLIENT_GetLevelName(levelNo));
	stream << levelName << ",";
	delete[] levelName;

	switch (presetType)
	{
	case UNIT_PLAYER:
		stream << "Player";
		break;
	case UNIT_MONSTER:
		stream << "Monster";
		break;
	case UNIT_OBJECT:
		stream << "Object";
		break;
	case UNIT_MISSILE:
		stream << "Missle";
		break;
	case UNIT_ITEM:
		stream << "Missle";
		break;
	case UNIT_TILE:
		stream << "Tile";
		break;
	case EXIT:
		stream << "Exit";
		break;
	case XY:
		stream << "XY";
		break;
	}
	if (presetType == UNIT_OBJECT && txtFileNo <= 572) {
		ObjectTxt* objTxt = D2COMMON_GetObjectTxt(txtFileNo);
		stream << " " << txtFileNo;
		if (objTxt && objTxt->szName) {
			stream << " " << objTxt->szName;
		}
		if (objTxt && objTxt->wszName) {
			char* name = UnicodeToAnsi(objTxt->wszName);
			stream << " (" << name << ")";
			delete[] name;
		}
		stream << ",";
	}
	else
	{
		stream << " " << txtFileNo << ",";
	}
	stream << levelRelativeRoomCoords.x << "," << levelRelativeRoomCoords.y << ",";
	stream << absoluteRoomCoords.x << "," << absoluteRoomCoords.y << ",";
	stream << roomRelativeCoords.x << "," << roomRelativeCoords.y << ",";
	stream << absoluteCoords.x << "," << absoluteCoords.y;
	stream << endl;
}

}