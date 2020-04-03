#include "Room2Aggregator.h"
#include "Common.h"
#include "D2Stubs.h"
#include "D2Ptrs.h"

using namespace std;

namespace MapInvestigation
{

void Room2Aggregator::onRoom2Load(Room2* room)
{
	levelNo = room->pLevel->dwLevelNo;
	roomFlags = room->dwRoomFlags;
	roomNumber = room->pType2Info->dwRoomNumber;
	subNumber = room->pType2Info->pdwSubNumber ? *room->pType2Info->pdwSubNumber : 0;
	presetType = room->dwPresetType;
	hasPreset = room->pPreset != NULL;
	txtFileNo = hasPreset ? room->pPreset->dwTxtFileNo : 0;
	absoluteRoomCoords = D2Coords(room->dwPosX, room->dwPosY);
	levelRelativeRoomCoords = absoluteRoomCoords - D2Coords(room->pLevel->pRoom2First->dwPosX, room->pLevel->pRoom2First->dwPosY);
	this->dumpData(*this->stream);
}

void Room2Aggregator::dumpHeader(ostream& stream)
{
	stream << "seed,level_name,room_flags,room_number,sub_number,preset_type,has_preset,preset_txt_number";
	stream << ",";
	stream << "level_relative_room_coords_x,level_relative_room_coords_y";
	stream << ",";
	stream << "absolute_room_coords_x,absolute_room_coords_y";
	stream << endl;
}

void Room2Aggregator::dumpData(ostream& stream)
{
	stream << seed << ",";

	char* levelName = UnicodeToAnsi(D2CLIENT_GetLevelName(levelNo));
	stream << levelName << ",";
	delete[] levelName;

	stream << roomFlags << ",";
	stream << roomNumber << ",";
	stream << subNumber << ",";
	stream << presetType << ",";
	stream << hasPreset << ",";
	stream << txtFileNo << ",";
	stream << levelRelativeRoomCoords.x << "," << levelRelativeRoomCoords.y;
	stream << ",";
	stream << absoluteRoomCoords.x << "," << absoluteRoomCoords.y;
	stream << endl;
}

}