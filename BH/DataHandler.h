#pragma once

#include "D2Structs.h"
#include <stdint.h>
#include <vector>
#include <iostream>

namespace MapInvestigation
{

class DataHandler {
public:
	virtual void onNewSeed(uint32_t seed) { this->seed = seed; };
	virtual void onSeedEnd(uint32_t seed) {};
	virtual void onLevelLoad(Level* level) {};
	virtual void onRoom2Load(Room2* room) {};
	virtual void onRoom1Load(Room1* room) {};
	virtual void onPresetUnitLoad(Room2* room, PresetUnit* presetUnit) {};

	uint32_t seed;
};

}