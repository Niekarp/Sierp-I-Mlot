#include "pch.h"
#include "IntroMenuAnimation.h"

IntroMenuAnimation::IntroMenuAnimation(IConsolePlane::Position menu_size) :
	_menu_sz(menu_size),
	_chr(' '),
	_color(0),
	_current_frame_index(0),
	_end(false)
{
}

void IntroMenuAnimation::draw(const std::shared_ptr<Console::Buffer>& buffer)
{
	if (_current_frame_index > _max_frame_index)
	{
		_end = true;
		return;
	}

	auto cx = buffer->screen_width() / 2;
	auto cy = buffer->screen_height() / 2;
	auto bx = cx - _current_frame_index * _menu_sz.x / 2 / _max_frame_index;
	auto by = cy - _current_frame_index * _menu_sz.y / 2 / _max_frame_index;
	auto ex = cx + _current_frame_index * _menu_sz.x / 2 / _max_frame_index;
	auto ey = cy + _current_frame_index * _menu_sz.y / 2 / _max_frame_index;

	for (auto i = bx; i < ex; ++i)
	{
		for (auto j = by; j < ey; ++j)
		{
			buffer->put(i, j, _chr, _color);
		}
	}

	_current_frame_index += 4;
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
