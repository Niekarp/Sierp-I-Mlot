#include "pch.h"
#include "HeroLifeBar.h"

static const int PROGRESS_COLORS_SIZE = 6;

static const WORD PROGRESS_COLORS[PROGRESS_COLORS_SIZE] =
{
	BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY,
	BACKGROUND_BLUE | BACKGROUND_INTENSITY,
	BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY,
	BACKGROUND_GREEN | BACKGROUND_INTENSITY,
	BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY,
	BACKGROUND_RED | BACKGROUND_INTENSITY,
};

using namespace xo;

xo::HeroLifeBar::HeroLifeBar() :
	_size({ 0, 0 }),
	_pos({ 0, 0 }),
	_frame_chr(' '),
	_frame_color(0),
	_progress(0)
{
}

HeroLifeBar::Position HeroLifeBar::position()
{
	return _pos;
}

HeroLifeBar::Position HeroLifeBar::size()
{
	return _size;
}

HeroLifeBar::PlaneType HeroLifeBar::type()
{
	return PlaneType::POSITIONABLE;
}

inline static int _get_progress_bar_left_frame_x(int iy, int by)
{
	return (iy - by) / 1.6 - POW2((iy - by) / 10.0);
}

void xo::HeroLifeBar::draw(const std::shared_ptr<Console::Buffer> &buffer)
{
	auto screen_w = buffer->screen_width();
	auto screen_h = buffer->screen_height();

	auto bx = screen_w - _size.x - _pos.x;
	auto by = screen_h / 2 - _size.y / 2;
	auto ex = bx + _size.x;
	auto ey = screen_h / 2 + _size.y / 2;

	// frame
	for (auto ix = bx; ix <= ex; ++ix)
	{
		buffer->put(ix, by, _frame_chr, _frame_color);
	}

	for (auto ix = bx + _get_progress_bar_left_frame_x(ey, by); ix <= ex; ++ix)
	{
		buffer->put(ix, ey, _frame_chr, _frame_color);
	}

	for (auto iy = by; iy <= ey; ++iy)
	{
		buffer->put(bx + _get_progress_bar_left_frame_x(iy, by), iy, _frame_chr, _frame_color);
		buffer->put(ex, iy, _frame_chr, _frame_color);
	}

	// progress
	for (auto iy = by; iy <= ey - 1; ++iy)
	{
		for (auto ix = bx + _get_progress_bar_left_frame_x(iy, by) + 1; ix <= ex - 1; ++ix)
		{
			if (iy < (int)(by + (ey - by) * (1 - _progress)) + 1)
			{
				buffer->put(ix, iy, ' ', 0);
			}
			else
			{
				auto color_idx = PROGRESS_COLORS_SIZE * (iy - by) / (ey - by);
				buffer->put(ix, iy, ' ', PROGRESS_COLORS[color_idx]);
			}
		}
	}

	// upper dividing line
	for (auto ix = bx + _get_progress_bar_left_frame_x(by + .25f * (ey - by), by); ix <= ex; ++ix)
	{
		buffer->put(ix, by + .25f * (ey - by), _frame_chr, BACKGROUND_RED );
	}
}

void xo::HeroLifeBar::size(Position sz)
{
	_size = sz;
}

void xo::HeroLifeBar::position(Position pos)
{
	_pos = pos;
}

void xo::HeroLifeBar::frame(char chr, WORD color)
{
	_frame_chr = chr;
	_frame_color = color;
}

void xo::HeroLifeBar::progress(float progress)
{
	_progress = progress;
}
