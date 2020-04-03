#pragma once

#include "DataAggregator.h"
#include <iostream>

namespace MapInvestigation
{

class DataDispatcher : public DataHandler
{
public:
	void add(DataAggregator* aggregator, const char* outputFilename);
	void dumpHeaders();
	void dumpData();

private:
	std::vector<DataAggregator*> aggregators;
	std::vector<std::unique_ptr<std::ostream>> files;
public:
	void onNewSeed(uint32_t seed) override {
		for (auto aggregator : aggregators)
			aggregator->onNewSeed(seed);
	}
	void onSeedEnd(uint32_t seed) override {
		for (auto aggregator : aggregators)
			aggregator->onSeedEnd(seed);
	}
	void onLevelLoad(Level* level) override {
		for (auto aggregator : aggregators)
			aggregator->onLevelLoad(level);
	}
	void onRoom2Load(Room2* room) override {
		for (auto aggregator : aggregators)
			aggregator->onRoom2Load(room);
	}
	void onRoom1Load(Room1* room) override {
		for (auto aggregator : aggregators)
			aggregator->onRoom1Load(room);
	}
	void onPresetUnitLoad(Room2* room, PresetUnit* presetUnit) override {
		for (auto aggregator : aggregators)
			aggregator->onPresetUnitLoad(room, presetUnit);
	}
};

}
