#include "DataDispatcher.h"
#include "BH.h"
#include <fstream>

namespace MapInvestigation
{

void DataDispatcher::add(DataAggregator* aggregator, const char* outputFilename)
{
	auto stream = std::make_unique<ofstream>(BH::path + "/" + outputFilename, ios_base::out | ios_base::trunc);
	if (stream->fail()) {
		cout << "Failed to open " << BH::path + "/" + outputFilename << endl;
		return;
	}
	aggregators.push_back(aggregator);
	aggregator->setStream(stream.get()); // FIXME: this is bad
	files.push_back(std::move(stream));
}

void DataDispatcher::dumpHeaders() {
	for (size_t index = 0; index < aggregators.size(); ++index)
	{
		auto stream = files[index].get();
		aggregators[index]->dumpHeader(*stream);
	}
}
void DataDispatcher::dumpData() {
	for (size_t index = 0; index < aggregators.size(); ++index)
	{
		auto stream = files[index].get();
		aggregators[index]->dumpData(*stream);
	}
}

}