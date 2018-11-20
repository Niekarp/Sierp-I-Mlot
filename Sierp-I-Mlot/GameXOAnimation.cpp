#include "pch.h"
#include "GameXOAnimation.h"

static const WORD _BC_1 = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
static const WORD _BC_0 = BACKGROUND_BLUE | BACKGROUND_INTENSITY;
static const int BACKGROUND_PATTERN_HEIGHT = 7;
static const int BACKGROUND_PATTERN_WIDTH = 8;
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

static const int BACKGROUND_PATTERN[2][BACKGROUND_PATTERN_HEIGHT][BACKGROUND_PATTERN_WIDTH] =
{
	{
		{ 1, 0, 0, 0, 1, 0, 0, 0 },
		{ 0, 1, 0, 1, 0, 0, 0, 0 },
		{ 0, 0, 1, 0, 0, 0, 0, 0 },
		{ 0, 1, 0, 1, 0, 0, 0, 0 },
		{ 1, 0, 0, 0, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
	},
	{
		{ 0, 1, 1, 1, 0, 0, 0, 0 },
		{ 1, 0, 0, 0, 1, 0, 0, 0 },
		{ 1, 0, 0, 0, 1, 0, 0, 0 },
		{ 1, 0, 0, 0, 1, 0, 0, 0 },
		{ 0, 1, 1, 1, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
	}
};

static struct _animation_helper_xo
{
	const std::shared_ptr<Console::Buffer> &buffer;
	int screen_w;
	int screen_h;
	int frame;
	int frame_start;
	int n_rows;
	float wave_cof;
	xo::PlayerSymbol player_symbol;

private:
	WORD _get_color(int i, int j)
	{
		auto ii = i * sin(frame / 64.);
		auto jj = j * cos(frame / 64.);

		auto center_len = max(abs(ii - screen_w / 2), abs(2 * (jj - screen_h / 2)));
		if (center_len < frame_start - frame)
		{
			return 0;
		}
		auto pattern = BACKGROUND_PATTERN[(int)player_symbol][j % BACKGROUND_PATTERN_HEIGHT][i % BACKGROUND_PATTERN_WIDTH];
		auto color_pos = center_len + frame / 2.;
		auto color = BACKGROUND_COLORS[0][(int)color_pos / BACKGROUND_PATTERN_HEIGHT % BACKGROUND_COLORS_SIZE];
		if (pattern) color |= BACKGROUND_INTENSITY;
		return color;
	}


	void _draw_single_row(int current_row)
	{
		auto screen_alignment_h = screen_h / n_rows * n_rows;
		auto height_offset = current_row * screen_alignment_h / n_rows + screen_h % n_rows / 2;

		for (int i = 0; i < screen_w; ++i)
		{
			double cy;
			if (current_row % 2 == 0)
			{
				cy = (sin(M_PI * (i + (int)frame / 2) / wave_cof) + 1) * screen_alignment_h / 2 / n_rows;
			}
			else
			{
				cy = (sin(M_PI * (i - (int)frame / 2) / wave_cof) + 1) * screen_alignment_h / 2 / n_rows;
			}

			for (int j = 0; j < screen_alignment_h / n_rows; ++j)
			{
				if (current_row % 2 == 0 && j > cy)
				{
					auto color = _get_color(i, j + height_offset);
					if (color != 0)
					{
						buffer->put(i, j + height_offset, ' ', color ^ BACKGROUND_INTENSITY);
					}
				}
				else if (current_row % 2 != 0 && j < cy)
				{
					auto color = _get_color(i, j + height_offset);
					if (color != 0)
					{
						buffer->put(i, j + height_offset, ' ', color ^ BACKGROUND_INTENSITY);
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
			_draw_single_row(i);
		}
	}
};

xo::GameXOAnimation::GameXOAnimation(int start_frame) :
	_speed(1),
	_start_frame(start_frame),
	_player_symbol(PlayerSymbol::circle)
{
}

void xo::GameXOAnimation::draw(const std::shared_ptr<Console::Buffer>& buffer, size_t frame)
{
	frame = frame * _speed + 400;

	auto screen_w = buffer->screen_width();
	auto screen_h = buffer->screen_height();

	_animation_helper_xo anim_helper{ buffer,	// const std::shared_ptr<Console::Buffer> &buffer;
		screen_w,								// int screen_w;
		screen_h,								// int screen_h;
		frame,									// int frame;
		_start_frame,							// int frame_start;
		10,										// int n_rows;
		20.f,									// float wave_cof;
		_player_symbol };									// int turn;
	

	anim_helper.draw();
}

bool xo::GameXOAnimation::end()
{
	return false;
}

bool xo::GameXOAnimation::continue_()
{
	return true;
}

void xo::GameXOAnimation::speed(int s)
{
	_speed = s;
}

void xo::GameXOAnimation::player_symbol(xo::PlayerSymbol s)
{
	_player_symbol = s;
}
