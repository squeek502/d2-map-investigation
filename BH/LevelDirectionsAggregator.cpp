#include "LevelDirectionsAggregator.h"
#include "Common.h"
#include "D2Stubs.h"
#include "D2Ptrs.h"
#include "MapInvestigation.h"

using namespace std;

namespace MapInvestigation
{

void LevelDirectionsAggregator::onLevelLoad(Level* level)
{
	toLevelNo = level->dwLevelNo;
	toCoords = D2Coords(level->pRoom2First->dwPosX, level->pRoom2First->dwPosY);
	if (level->dwLevelNo != actStartingLevelIds[level->pMisc->pAct->dwAct])
		this->dumpData(*this->stream);
	fromLevelNo = toLevelNo;
	fromCoords = toCoords;
}

void LevelDirectionsAggregator::dumpHeader(ostream& stream)
{
	stream << "seed,from_level,to_level,direction";
	stream << endl;
}

void LevelDirectionsAggregator::dumpData(ostream& stream)
{
	stream << seed << ",";

	char* fromLevelName = UnicodeToAnsi(D2CLIENT_GetLevelName(fromLevelNo));
	stream << fromLevelName << ",";
	delete[] fromLevelName;

	char* toLevelName = UnicodeToAnsi(D2CLIENT_GetLevelName(toLevelNo));
	stream << toLevelName << ",";
	delete[] toLevelName;

	stream << DirectionString(toCoords.directionFrom(fromCoords));
	stream << endl;
}

}