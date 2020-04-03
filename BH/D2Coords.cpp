#include "D2Coords.h"
#include <stdlib.h>

namespace MapInvestigation
{

const char * DirectionString(Direction dir)
{
	switch (dir)
	{
		case Unknown: return "Unknown";
		case TopRight: return "TopRight";
		case BottomRight: return "BottomRight";
		case BottomLeft: return "BottomLeft";
		case TopLeft: return "TopLeft";
		case Top: return "Top";
		case Right: return "Right";
		case Bottom: return "Bottom";
		case Left: return "Left";
		default: return "Error";
	}
}

Direction GetDirection(int64_t fromX, int64_t fromY, int64_t toX, int64_t toY)
{
	int64_t deltaX = toX - fromX;
	int64_t deltaY = toY - fromY;
	return GetDirectionFromDelta(deltaX, deltaY);
}

Direction GetDirectionFromDelta(int64_t deltaX, int64_t deltaY)
{
	if (abs(deltaX) == abs(deltaY))
	{
		if (deltaX > 0 && deltaY > 0) return Direction::Bottom;
		if (deltaX > 0 && deltaY < 0) return Direction::Right;
		if (deltaX < 0 && deltaY < 0) return Direction::Top;
		if (deltaX < 0 && deltaY > 0) return Direction::Left;
	}
	else if (abs(deltaX) > abs(deltaY))
	{
		return deltaX > 0 ? Direction::BottomRight : Direction::TopLeft;
	}
	else
	{
		return deltaY > 0 ? Direction::BottomLeft : Direction::TopRight;
	}
	return Direction::Unknown;
}

}