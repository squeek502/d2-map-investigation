#pragma once

#include "DataAggregator.h"
#include "D2Coords.h"

namespace MapInvestigation
{

class Room2Aggregator : public DataAggregator
{
private:
	uint32_t levelNo;
	uint32_t roomFlags;
	uint32_t roomNumber;
	uint32_t subNumber;
	uint32_t presetType;
	bool hasPreset;
	uint32_t txtFileNo;
	D2Coords absoluteRoomCoords;
	D2Coords levelRelativeRoomCoords;
public:
	void onRoom2Load(Room2* room) override;

	void dumpHeader(std::ostream& stream) override;
	void dumpData(std::ostream& stream) override;
};

}
