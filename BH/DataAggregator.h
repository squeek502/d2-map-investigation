#pragma once

#include "DataHandler.h"
#include "DataDumper.h"

namespace MapInvestigation
{

class DataAggregator : public DataHandler, public DataDumper
{
};

}
