#pragma once

#include "DataAggregator.h"
#include "D2Coords.h"

namespace MapInvestigation
{

class ArcaneAggregator : public DataAggregator
{
private:
	Direction arcaneDirection = Direction::Unknown;
	Direction portalsDirection = Direction::Unknown;

public:
	void onNewSeed(uint32_t seed) override;
	void onSeedEnd(uint32_t seed) override;
	void onRoom2Load(Room2* room) override;
	void onPresetUnitLoad(Room2* room, PresetUnit* presetUnit) override;

	void dumpData(std::ostream& stream) override;
	void dumpHeader(std::ostream& stream) override;
};

}
