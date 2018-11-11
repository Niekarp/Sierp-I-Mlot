#pragma once
#include "Console.h"

struct IAnimation
{
	virtual void draw(const std::shared_ptr<Console::Buffer> &buffer, size_t frame) = 0;
	virtual bool end() = 0;
	virtual bool continue_() = 0;
};
