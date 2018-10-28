#include "stdafx.h"
#include "SuperWaveAnimation.h"

static const WORD _BC_1 = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
static const WORD _BC_0 = BACKGROUND_BLUE | BACKGROUND_INTENSITY;
static const int BACKGROUND_PATTERN_HEIGHT = 7;
static const int BACKGROUND_PATTERN_WIDTH = 14;
static const int BACKGROUND_COLORS_SIZE = 30;

static const WORD BACKGROUND_COLORS[2][BACKGROUND_COLORS_SIZE] =
{
	//0
	{
		BACKGROUND_RED | BACKGROUND_GREEN,
		BACKGROUND_RED | BACKGROUND_GREEN,
		BACKGROUND_RED | BACKGROUND_GREEN,
		BACKGROUND_RED | BACKGROUND_GREEN,
		BACKGROUND_RED | BACKGROUND_GREEN,

		BACKGROUND_GREEN | BACKGROUND_BLUE,
		BACKGROUND_GREEN | BACKGROUND_BLUE,
		BACKGROUND_GREEN | BACKGROUND_BLUE,
		BACKGROUND_GREEN | BACKGROUND_BLUE,
		BACKGROUND_GREEN | BACKGROUND_BLUE,

		BACKGROUND_RED | BACKGROUND_BLUE,
		BACKGROUND_RED | BACKGROUND_BLUE,
		BACKGROUND_RED | BACKGROUND_BLUE,
		BACKGROUND_RED | BACKGROUND_BLUE,
		BACKGROUND_RED | BACKGROUND_BLUE,

		BACKGROUND_RED,
		BACKGROUND_RED,
		BACKGROUND_RED,
		BACKGROUND_RED,
		BACKGROUND_RED,

		BACKGROUND_GREEN,
		BACKGROUND_GREEN,
		BACKGROUND_GREEN,
		BACKGROUND_GREEN,
		BACKGROUND_GREEN,

		BACKGROUND_BLUE,
		BACKGROUND_BLUE,
		BACKGROUND_BLUE,
		BACKGROUND_BLUE,
		BACKGROUND_BLUE,
	},

	//1
	{
		BACKGROUND_GREEN | BACKGROUND_BLUE,
		BACKGROUND_RED | BACKGROUND_BLUE,
		BACKGROUND_RED,
		BACKGROUND_GREEN,
		BACKGROUND_BLUE,

		BACKGROUND_RED | BACKGROUND_GREEN,
		BACKGROUND_RED | BACKGROUND_BLUE,
		BACKGROUND_RED,
		BACKGROUND_GREEN,
		BACKGROUND_BLUE,

		BACKGROUND_RED | BACKGROUND_GREEN,
		BACKGROUND_GREEN | BACKGROUND_BLUE,
		BACKGROUND_RED,
		BACKGROUND_GREEN,
		BACKGROUND_BLUE,

		BACKGROUND_RED | BACKGROUND_GREEN,
		BACKGROUND_GREEN | BACKGROUND_BLUE,
		BACKGROUND_RED | BACKGROUND_BLUE,
		BACKGROUND_GREEN,
		BACKGROUND_BLUE,

		BACKGROUND_RED | BACKGROUND_GREEN,
		BACKGROUND_GREEN | BACKGROUND_BLUE,
		BACKGROUND_RED | BACKGROUND_BLUE,
		BACKGROUND_RED,
		BACKGROUND_BLUE,

		BACKGROUND_RED | BACKGROUND_GREEN,
		BACKGROUND_GREEN | BACKGROUND_BLUE,
		BACKGROUND_RED | BACKGROUND_BLUE,
		BACKGROUND_RED,
		BACKGROUND_GREEN,
	}
};

static const int BACKGROUND_PATTERN[BACKGROUND_PATTERN_HEIGHT][BACKGROUND_PATTERN_WIDTH] =
{
	{ 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0 },
	{ 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 },
	{ 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 },
	{ 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 },
	{ 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

static inline WORD _get_color(int i, int j, int o, int screen_width, int screen_height)
{
	auto center_len = (int)sqrt((i - screen_width / 2) * (i - screen_width / 2) 
		+ 4 * (j - screen_height / 2) * (j - screen_height / 2));
	if (center_len < 1200 - o)
	{
		return 0;
	}
	auto pattern = BACKGROUND_PATTERN[j % BACKGROUND_PATTERN_HEIGHT][i % BACKGROUND_PATTERN_WIDTH];
	auto color_pos = center_len / 2 + o / 4;
	auto color = BACKGROUND_COLORS[pattern][color_pos / BACKGROUND_PATTERN_HEIGHT % BACKGROUND_COLORS_SIZE];
	return color;
}

static inline void _draw_single_row(const std::shared_ptr<Console::Buffer> &buffer,
	int screen_w, int screen_h, int n_rows, int current_row,
	int o)
{
	auto screen_alignment_h = screen_h / n_rows * n_rows;
	auto height_offset = current_row * screen_alignment_h / n_rows;

	for (int i = 0; i < screen_w; ++i)
	{
		double cy;
		if (current_row % 2 == 0)
		{
			cy = (sin(M_PI * (i + (int)o) / 60.f) + 1) * screen_alignment_h / 2 / n_rows;
		}
		else
		{
			cy = (sin(M_PI * (i - (int)o) / 60.f) + 1) * screen_alignment_h / 2 / n_rows;
		}

		for (int j = 0; j < screen_alignment_h / n_rows; ++j)
		{
			if (current_row % 2 == 0 && j > cy)
			{
				auto color = _get_color(i, j + height_offset, o,
					screen_w, screen_h);
				if (color != 0)
				{
					buffer->put(i, j + height_offset, ' ', color | BACKGROUND_INTENSITY);
				}
			}
			else if (current_row % 2 != 0 && j < cy)
			{
				auto color = _get_color(i, j + height_offset, o,
					screen_w, screen_h);
				if (color != 0)
				{
					buffer->put(i, j + height_offset, ' ', color | BACKGROUND_INTENSITY);
				}
			}
		}
	}
}

static inline void _draw_wave_background(const std::shared_ptr<Console::Buffer> &buffer,
	int screen_w, int screen_h, int o)
{
	for (int i = 0; i < screen_w; ++i)
	{
		for (int j = 0; j < screen_h; ++j)
		{
			auto color = _get_color(i, j, o, screen_w, screen_h);
			if (color != 0)
			{
				buffer->put(i, j, ' ', color);
			}
		}
	}
}

SuperWaveAnimation::SuperWaveAnimation(int n_rows) :
	_n_rows(n_rows)
{
}

void SuperWaveAnimation::draw(const std::shared_ptr<Console::Buffer>& buffer)
{
	static int o = 1000;

	auto screen_w = buffer->screen_width();
	auto screen_h = buffer->screen_height();

	_draw_wave_background(buffer, screen_w, screen_h, o);

	for (int i = 0; i < _n_rows; ++i)
	{
		_draw_single_row(buffer, screen_w, screen_h, _n_rows, i, o);
	}

	o += 4;
}

bool SuperWaveAnimation::end()
{
	return false;
}

bool SuperWaveAnimation::continue_()
{
	return true;
}
