#pragma once

#include "DataAggregator.h"
#include "D2Coords.h"

namespace MapInvestigation
{

class PresetsAggregator : public DataAggregator
{
public:
	PresetsAggregator() {}

private:
	uint32_t levelNo;
	uint32_t presetType;
	uint32_t txtFileNo;
	D2Coords absoluteRoomCoords;
	D2Coords levelRelativeRoomCoords;
	D2Coords absoluteCoords;
	D2Coords roomRelativeCoords;

public:
	void onPresetUnitLoad(Room2* room, PresetUnit* presetUnit) override;

	void dumpData(std::ostream& stream) override;
	void dumpHeader(std::ostream& stream) override;
};

}
