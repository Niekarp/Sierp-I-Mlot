#pragma once
#include "Console.h"

struct IConsolePlane
{
	struct Position
	{
		int x;
		int y;
	};
	
	enum struct PlaneType
	{
		POSITIONABLE,
		CENTERED,
		STRETCHABLE
	};
	
	virtual Position position() = 0;
	virtual Position size() = 0;
	virtual PlaneType type() = 0;
	virtual void draw(const std::shared_ptr<Console::Buffer> &) = 0;
};

