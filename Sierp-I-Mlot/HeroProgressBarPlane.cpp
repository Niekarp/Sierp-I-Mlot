#include "pch.h"
#include "HeroProgressBarPlane.h"

static const int PARTICLE_MAX_R = 20;
static const int PARTICLE_MIN_R = 6;
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

HeroProgressBarPlane::HeroProgressBarPlane() :
	_position({0, 0}),
	_size({0, 0}),
	_frame_chr(' '),
	_frame_color(0),
	_frame(0)
{
}

HeroProgressBarPlane::Position HeroProgressBarPlane::position()
{
	return _position;
}

HeroProgressBarPlane::Position HeroProgressBarPlane::size()
{
	return _size;
}

HeroProgressBarPlane::PlaneType HeroProgressBarPlane::type()
{
	return PlaneType::POSITIONABLE;
}

void xo::HeroProgressBarPlane::draw(const std::shared_ptr<Console::Buffer> &buffer)
{
	auto screen_w = buffer->screen_width();
	auto screen_h = buffer->screen_height();


	auto bx = 0;
	auto by = _position.y;
	auto ex = screen_w - 1;
	auto ey = by + _size.y;


	for (auto ix = bx + (screen_w / 2 - bx) * (1.f -_progress); ix <= screen_w / 2 + (ex - screen_w / 2) * _progress ; ++ix)
	{
   		auto cy = by + (ey - by) * (sin((ix - _frame / 4.0) / 4.0) + 1) / 2;
		for (auto iy = cy - 1; iy <= cy + 1; ++iy)
		{
			buffer->put(ix, iy, _frame_chr, PROGRESS_COLORS[abs((int)((ix - _frame / 4.0) / 4.0)) % PROGRESS_COLORS_SIZE] ^ BACKGROUND_INTENSITY);
		}

		buffer->put(ix, cy, _frame_chr, PROGRESS_COLORS[ abs((int)((ix - _frame / 4.0) / 4.0)) % PROGRESS_COLORS_SIZE]);
	}

	auto p1cx = bx + (screen_w / 2 - bx) * (1.f - _progress);
	auto p1cy = by + (ey - by) * (sin((p1cx - _frame / 4.0) / 4.0) + 1) / 2 - 1;
	auto p2cx = screen_w / 2 + (ex - screen_w / 2) * _progress;
	auto p2cy = by + (ey - by) * (sin((p2cx - _frame / 4.0) / 4.0) + 1) / 2 - 1;

	auto note_r = 10;
	auto nbx = p1cx - note_r;
	auto pby = p1cy - 2 * note_r / 5;
	auto pex = p1cx + note_r;
	auto pey = p1cy + 2 * note_r / 5;

	for (auto ix = nbx; ix <= pex; ++ix)
	{
		for (auto iy = pby; iy <= pey; ++iy)
		{
			
			//buffer->put(ix, iy, ' ', BACKGROUND_BLUE);
		}
	}


	/*for (auto ix = bx; ix <= ex; ++ix)
	{
		buffer->put(ix, by, _frame_chr, _frame_color);
		buffer->put(ix, ey, _frame_chr, _frame_color);
	}


	for (auto iy = by; iy <= ey; ++iy)
	{
		buffer->put(bx, iy, _frame_chr, _frame_color);
		buffer->put(ex, iy, _frame_chr, _frame_color);
	}

	for (auto ix = bx + 1; ix <= ex - 1; ++ix)
	{
		for (auto iy = by + 1; iy <= ey - 1; ++iy)
		{
			buffer->put(ix, iy, _frame_chr, _frame_color);
		}
	}*/

	_frame += 1;
}

void HeroProgressBarPlane::frame(char chr, WORD color)
{
	_frame_chr = chr;
	_frame_color = color;
}

void HeroProgressBarPlane::position(Position pos)
{
	_position = pos;
}

void HeroProgressBarPlane::size(Position s)
{
	_size = s;
}

void xo::HeroProgressBarPlane::progress(float p)
{
	_progress = p;
}
