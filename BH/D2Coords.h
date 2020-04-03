#pragma once

#include "stdint.h"

namespace MapInvestigation
{

enum Direction
{
	Unknown,
	TopRight,
	BottomRight,
	BottomLeft,
	TopLeft,
	Top,
	Right,
	Bottom,
	Left,
};

const char* DirectionString(Direction dir);
Direction GetDirection(int64_t fromX, int64_t fromY, int64_t toX, int64_t toY);
Direction GetDirectionFromDelta(int64_t deltaX, int64_t deltaY);

// forward dec
class D2Coords
{
public:
	int64_t x;
	int64_t y;

	D2Coords() : x(0), y(0) {}
	D2Coords(int64_t _x, int64_t _y) : x(_x), y(_y) {}

	D2Coords operator - (D2Coords const &rhs) {
		return D2Coords(x - rhs.x, y - rhs.y);
	}

	D2Coords operator + (D2Coords const &rhs) {
		return D2Coords(x + rhs.x, y + rhs.y);
	}

	// convert 'room coords' to 'tile coords'
	D2Coords toTileCoords() {
		return D2Coords(x * 5, y * 5);
	}

	Direction directionFrom(D2Coords from)
	{
		return GetDirection(from.x, from.y, this->x, this->y);
	}
};

}