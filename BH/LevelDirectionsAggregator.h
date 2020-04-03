#pragma once

#include "DataAggregator.h"
#include "D2Coords.h"

namespace MapInvestigation
{

class LevelDirectionsAggregator : public DataAggregator
{
public:
	LevelDirectionsAggregator() {}

private:
	uint32_t fromLevelNo;
	uint32_t toLevelNo;
	D2Coords fromCoords;
	D2Coords toCoords;

public:
	void onLevelLoad(Level* level) override;

	void dumpData(std::ostream& stream) override;
	void dumpHeader(std::ostream& stream) override;
};

}
