#include "pch.h"
#include "IntroMenuAnimation.h"

IntroMenuAnimation::IntroMenuAnimation(IConsolePlane::Position menu_size) :
	_menu_sz(menu_size),
	_chr(' '),
	_color(0),
	_end(false)
{
}

void IntroMenuAnimation::draw(const std::shared_ptr<Console::Buffer>& buffer, size_t frame)
{
	frame *= 2;

	if (frame > _max_frame_index)
	{
		_end = true;
		return;
	}

	auto cx = buffer->screen_width() / 2;
	auto cy = buffer->screen_height() / 2;
	auto bx = cx - frame * _menu_sz.x / 2 / _max_frame_index;
	auto by = cy - frame * _menu_sz.y / 2 / _max_frame_index;
	auto ex = cx + frame * _menu_sz.x / 2 / _max_frame_index;
	auto ey = cy + frame * _menu_sz.y / 2 / _max_frame_index;

	for (auto i = bx; i < ex; ++i)
	{
		for (auto j = by; j < ey; ++j)
		{
			buffer->put(i, j, _chr, _color);
		}
	}
}

bool IntroMenuAnimation::end()
{
	return _end;
}

bool IntroMenuAnimation::continue_()
{
	return false;
}

void IntroMenuAnimation::fill(char chr, WORD color)
{
	_chr = chr;
	_color = color;
}
