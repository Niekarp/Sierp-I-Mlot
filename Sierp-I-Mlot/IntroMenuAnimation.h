#pragma once
#include "IAnimation.h"
#include "IConsolePlane.h"

struct IntroMenuAnimation :
	public IAnimation
{
	IntroMenuAnimation(IConsolePlane::Position menu_size);
	void draw(const std::shared_ptr<Console::Buffer> &buffer, size_t frame) override;
	bool end() override;
	bool continue_() override;

	void fill(char chr, WORD color);

private:
	static const size_t _max_frame_index = 100;

	IConsolePlane::Position _menu_sz;
	char _chr;
	WORD _color;
	bool _end;
};

