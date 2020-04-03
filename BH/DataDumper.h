#pragma once

#include "D2Structs.h"
#include <stdint.h>
#include <vector>
#include <iostream>

namespace MapInvestigation
{

class DataDumper {
protected:
	std::ostream* stream;

public:
	virtual void dumpData(std::ostream& stream) = 0;
	virtual void dumpHeader(std::ostream& stream) = 0;
	void setStream(std::ostream* stream) {
		this->stream = stream;
	}
};

}