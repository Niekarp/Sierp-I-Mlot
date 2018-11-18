#include "pch.h"
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


static struct _animation_helper
{
	const std::shared_ptr<Console::Buffer> &buffer;
	int screen_w;
	int screen_h;
	int frame;
	int frame_start;
	int n_rows;
	int center_screen_x;
	int center_screen_y;

private:
	WORD _get_color(int i, int j)
	{
		auto center_len = (int)sqrt((i - center_screen_x) * (i - center_screen_x)
			+ 4 * (j - center_screen_y) * (j - center_screen_y));
		if (center_len < frame_start - frame)
		{
			return 0;
		}
		auto pattern = BACKGROUND_PATTERN[j % BACKGROUND_PATTERN_HEIGHT][i % BACKGROUND_PATTERN_WIDTH];
		auto color_pos = center_len / 2 + frame / 4;
		auto color = BACKGROUND_COLORS[pattern][color_pos / BACKGROUND_PATTERN_HEIGHT % BACKGROUND_COLORS_SIZE];
		return color;
	}

	WORD _get_color2(int i, int j)
	{
		auto cx = screen_w / 2 * (4 * sin(frame / 1024.0) + 1);
		auto cy = screen_h / 2 * (4 * cos(frame / 1024.0) + 1);

		auto center_len = (int)sqrt((i - cx) * (i - cx)
			+ 4 * (j - cy) * (j - cy));
		if (center_len < frame_start - frame)
		{
			return 0;
		}
		auto pattern = BACKGROUND_PATTERN[(j + frame / 32) % BACKGROUND_PATTERN_HEIGHT][(i + frame / 32) % BACKGROUND_PATTERN_WIDTH];
		auto color_pos = center_len / 2 + frame / 4;
		auto color = BACKGROUND_COLORS[pattern][color_pos / BACKGROUND_PATTERN_HEIGHT % BACKGROUND_COLORS_SIZE];
		return color;
	}


	void _draw_single_row(int n_rows, int current_row)
	{
		auto screen_alignment_h = screen_h / n_rows * n_rows;
		auto height_offset = current_row * screen_alignment_h / n_rows;

		for (int i = 0; i < screen_w; ++i)
		{
			double cy;
			if (current_row % 2 == 0)
			{
				cy = (sin(M_PI * (i + (int)frame) / 60.f) + 1) * screen_alignment_h / 2 / n_rows;
			}
			else
			{
				cy = (sin(M_PI * (i - (int)frame) / 60.f) + 1) * screen_alignment_h / 2 / n_rows;
			}

			for (int j = 0; j < screen_alignment_h / n_rows; ++j)
			{
				if (current_row % 2 == 0 && j > cy)
				{
					auto color = _get_color(i, j + height_offset);
					if (color != 0)
					{
						buffer->put(i, j + height_offset, ' ', color | BACKGROUND_INTENSITY);
					}
				}
				else if (current_row % 2 != 0 && j < cy)
				{
					auto color = _get_color(i, j + height_offset);
					if (color != 0)
					{
						buffer->put(i, j + height_offset, ' ', color | BACKGROUND_INTENSITY);
					}
				}
			}
		}
	}

public:
	void draw()
	{
		for (int i = 0; i < screen_w; ++i)
		{
			for (int j = 0; j < screen_h; ++j)
			{
				auto color = _get_color(i, j);
				if (color != 0)
				{
					buffer->put(i, j, ' ', color);
				}
			}
		}

		for (int i = 0; i < n_rows; ++i)
		{
			_draw_single_row(n_rows, i);
		}
	}
};

SuperWaveAnimation::SuperWaveAnimation(int start_frame, int n_rows) :
	_start_frame(start_frame),
	_n_rows(n_rows),
	_speed(1),
	_center_x(0),
	_center_y(0)
{
}

void SuperWaveAnimation::draw(const std::shared_ptr<Console::Buffer>& buffer, size_t frame)
{
	frame = frame * _speed + 400;

	auto screen_w = buffer->screen_width();
	auto screen_h = buffer->screen_height();

	_animation_helper anim_helper{ 
		buffer,
		screen_w, 
		screen_h, 
		frame,
		_start_frame,
		_n_rows,
		screen_w / 2 + _center_x,
		screen_h / 2 + _center_y
	};

	anim_helper.draw();
}

bool SuperWaveAnimation::end()
{
	return false;
}

bool SuperWaveAnimation::continue_()
{
	return true;
}

void SuperWaveAnimation::speed(int s)
{
	_speed = s;
}

void SuperWaveAnimation::center(int x, int y)
{
	_center_x = x;
	_center_y = y;
}
