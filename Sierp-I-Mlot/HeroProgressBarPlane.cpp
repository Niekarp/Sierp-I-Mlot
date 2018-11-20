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
static const long long PARTICLES_TIME[] =
{
	0,
	50,
	100,
	150,
	200,
	250,
	300,
};


using namespace xo;

HeroProgressBarPlane::HeroProgressBarPlane() :
	_position({ 0, 0 }),
	_size({ 0, 0 }),
	_frame_chr(' '),
	_frame_color(0),
	_frame(0),
	_progress(.0f)
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


	for (auto ix = bx + (screen_w / 2 - bx) * (1.f - _progress); ix <= screen_w / 2 + (ex - screen_w / 2) * _progress; ++ix)
	{
		auto cy = by + (ey - by) * (sin((ix - _frame / 4.0) / 4.0) + 1) / 2;
		for (auto iy = cy - 1; iy <= cy + 1; ++iy)
		{
			buffer->put(ix, iy, _frame_chr, PROGRESS_COLORS[abs((int)((ix + _frame / 4.0) / 4.0)) % PROGRESS_COLORS_SIZE] ^ BACKGROUND_INTENSITY);
		}

		buffer->put(ix, cy, _frame_chr, PROGRESS_COLORS[abs((int)((ix + _frame / 4.0) / 4.0)) % PROGRESS_COLORS_SIZE]);
	}


	auto p1cx = bx + (screen_w / 2 - bx) * (1.f - _progress);
	auto p1cy = by + (ey - by) * (sin((p1cx - _frame / 4.0) / 4.0) + 1) / 2 - 1;
	auto p2cx = screen_w / 2 + (ex - screen_w / 2) * _progress;
	auto p2cy = by + (ey - by) * (sin((p2cx - _frame / 4.0) / 4.0) + 1) / 2 - 1;

	auto note_r = 10;

	auto p1bx = p1cx - note_r;
	auto p1by = p1cy - 2 * note_r / 5;
	auto p1ex = p1cx + note_r;
	auto p1ey = p1cy + 2 * note_r / 5;

	auto p2bx = p2cx - note_r;
	auto p2by = p2cy - 2 * note_r / 5;
	auto p2ex = p2cx + note_r;
	auto p2ey = p2cy + 2 * note_r / 5;


	_draw_particles(buffer, p1bx, p1by, p1ex, p1ey, p1cx, p1cy);
	_draw_particles(buffer, p2bx, p2by, p2ex, p2ey, p2cx, p2cy);

	


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
	if (isinf(p))
	{
		p = 1.f;
	}
	_progress = p;
}

void xo::HeroProgressBarPlane::_draw_particles(const std::shared_ptr<Console::Buffer> &buffer, float nbx,
	float pby, float pex, float pey, float p1cx, float p1cy)
{
	auto note_r = 10;

	for (auto ip = (_frame - 300) / 40 * 40; ip < (_frame + 300) / 40 * 40; ip += 40)
	{
		if (ip < 0)
		{
			continue;
		}
		if (ip > _frame)
		{
			continue;
		}
		for (auto ix = nbx; ix <= pex; ++ix)
		{
			for (auto iy = pby; iy <= pey; ++iy)
			{
				auto r = sqrt((ix - p1cx) * (ix - p1cx) + 4 * (iy - p1cy) * (iy - p1cy));

				auto p1color = PROGRESS_COLORS[abs((int)((p1cx + ip / 4.0) / 4.0)) % PROGRESS_COLORS_SIZE];
				//auto p2color = PROGRESS_COLORS[abs((int)((p2cx + _frame / 4.0) / 4.0)) % PROGRESS_COLORS_SIZE];

				double normalized_x;
				double normalized_y;
				if (r == 0)
				{

					normalized_x = 0;
					normalized_y = 0;
				}
				else
				{

					normalized_x = (ix - p1cx) / r;
					normalized_y = (iy - p1cy) / r;
				}

				auto ix2 = ix + normalized_x * (_frame - ip) / 3;
				auto iy2 = iy + normalized_y * (_frame - ip) / 3;

				if ((ix2 - p1cx) * (ix2 - p1cx) + 4 * (iy2 - p1cy) * (iy2 - p1cy) > 400)
				{
					continue;
				}

				if (r > 3 * note_r / 4)
				{
					continue;
				}
				else if (r >= 3 * note_r / 4 - 1)
				{
					buffer->put(ix2, iy2, ' ', p1color ^ BACKGROUND_INTENSITY);
					continue;
				}
				buffer->put(ix2, iy2, ' ', p1color);
			}
		}
	}
}
