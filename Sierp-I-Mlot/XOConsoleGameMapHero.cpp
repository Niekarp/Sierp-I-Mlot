#include "pch.h"
#include "XOConsoleGameMapHero.h"
#include "AnimationChain.h"
#include "GameHeroAnimation.h"

static const int MAP_WIDTH_TOP = 30;
static const int MAP_WIDTH_BOTTOM = 80;
static const WORD NOTE_COLORS[] = {
	BACKGROUND_BLUE,
	BACKGROUND_GREEN,
	BACKGROUND_RED | BACKGROUND_GREEN,
	BACKGROUND_RED | BACKGROUND_BLUE
};
static const int NOTE_MAX_R = 20;
static const int NOTE_MIN_R = 6;
static const int NOTE_START_TIME = 3000;


using namespace xo;

XOConsoleGameMapHero::XOConsoleGameMapHero() :
	_key_down_callback(std::bind(&XOConsoleGameMapHero::_on_key,
		this, std::placeholders::_1, true)),
	_key_up_callback(std::bind(&XOConsoleGameMapHero::_on_key,
		this, std::placeholders::_1, false)),
	_key_callback_binded(false),
	_frame(0),
	_omission_time(0),
	_play(false)
{
	_note_image = std::make_shared<FileImagePlane>(conf::FILE_NOTE);
	_note_image->color(FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);

	_life_bar = std::make_shared<HeroLifeBar>();
	_life_bar->size({ conf::GAME_HERO_LIFE_BAR_SIZE_X, conf::GAME_HERO_LIFE_BAR_SIZE_Y });
	_life_bar->position({ conf::GAME_HERO_LIFE_BAR_OFFSET_X, 0 });
	_life_bar->progress(1.);
	_life_bar->frame(' ', 0);

	_progress_bar = std::make_shared<HeroProgressBarPlane>();
	_progress_bar->size({ conf::GAME_HERO_PROGRESS_BAR_SIZE_X, conf::GAME_HERO_PROGRESS_BAR_SIZE_Y });
	_progress_bar->position({ 0, conf::GAME_HERO_PROGRESS_BAR_OFFSET_Y });
	_progress_bar->frame(' ', 0);
	_progress_bar->progress(1.);
}


XOConsoleGameMapHero::~XOConsoleGameMapHero()
{
}

void XOConsoleGameMapHero::register_element(const XOViewElement &element)
{
	_elements.push_back(element);
}

void xo::XOConsoleGameMapHero::start(long long start, long long omitt, long long duration)
{
	_omission_time = omitt;
	_duration = duration;
	_start_point = std::chrono::system_clock::now();
	_play = true;
}

XOConsoleGameMapHero::Position XOConsoleGameMapHero::position()
{
	return Position();
}

XOConsoleGameMapHero::Position XOConsoleGameMapHero::size()
{
	return Position();
}

XOConsoleGameMapHero::PlaneType XOConsoleGameMapHero::type()
{
	return PlaneType::CENTERED;
}

inline static int _get_guitar_string_x(int y, int string_n,
	int screen_width, int screen_height)
{
	float ey = screen_height - 1;
	float wx = MAP_WIDTH_TOP + y * (MAP_WIDTH_BOTTOM - MAP_WIDTH_TOP) / ey - 15;
	float bx = screen_width / 2 - wx / 2;

	return (int)(bx + (string_n + 0.5f) * wx / N_STRINGS);
}

static struct anim_helper_hero_map
{
	const std::shared_ptr<Console::Buffer>&buffer;
	const std::shared_ptr<NoteContainer> &notes;
	long long duration;
	long long ommision_time;

	void _draw_guitar_neck(int by)
	{
		auto ey = buffer->screen_height() - 1;

		for (auto iy = by; iy <= ey; ++iy)
		{
			auto wx = MAP_WIDTH_TOP + iy * (MAP_WIDTH_BOTTOM - MAP_WIDTH_TOP) / ey;
			auto bx = buffer->screen_width() / 2 - wx / 2;
			auto ex = buffer->screen_width() / 2 + wx / 2;

			buffer->put(bx, iy, ' ', BACKGROUND_RED);
			buffer->put(bx + 1, iy, ' ', BACKGROUND_RED);
			buffer->put(ex, iy, ' ', BACKGROUND_RED);
			buffer->put(ex - 1, iy, ' ', BACKGROUND_RED);

			for (auto ix = bx + 2; ix <= ex - 2; ++ix)
			{
				buffer->put(ix, iy, ' ', BACKGROUND_RED | BACKGROUND_INTENSITY);
			}
		}
	}

	void _draw_guitar_strings(int by)
	{
		auto screen_width = buffer->screen_width();
		auto screen_height = buffer->screen_height();

		for (int iy = by; iy < buffer->screen_height(); ++iy)
		{
			for (int istring = 0; istring < N_STRINGS; ++istring)
			{
				auto ix = _get_guitar_string_x(iy, istring, screen_width, screen_height);
				buffer->put(ix, iy, ' ', 0);
			}
		}
	}

	void _put_note(const Note &note)//int string_n, int offset, long long destroy_time)
	{
		auto screen_width = buffer->screen_width();
		auto screen_height = buffer->screen_height();
		auto string_n = note.tone;
		auto offset = (note.time - duration) * screen_height / NOTE_START_TIME;
		auto destroy_time = note.destroy_time;

		auto cy = screen_height - 1 - offset;
		auto cx = _get_guitar_string_x(cy, string_n, screen_width, screen_height);

		auto note_r = (NOTE_MIN_R + cy * (NOTE_MAX_R - NOTE_MIN_R) / screen_height) / 2;
		auto bx = cx - note_r;
		auto by = cy - 2 * note_r / 5;
		auto ex = cx + note_r;
		auto ey = cy + 2 * note_r / 5;

		if (note.destroy_time != -1)
		{
			for (auto ix = bx; ix <= ex; ++ix)
			{
				for (auto iy = by; iy <= ey; ++iy)
				{
					auto r = sqrt((ix - cx) * (ix - cx) + 4 * (iy - cy) * (iy - cy));


					double normalized_x;
					double normalized_y;
					if (r == 0)
					{

						normalized_x = 0;
						normalized_y = 0;
					}
					else
					{

						normalized_x = (ix - cx) / r;
						normalized_y = (iy - cy) / r;
					}

					auto ix2 = ix + normalized_x * (duration - note.destroy_time) / 20;
					auto iy2 = iy + normalized_y * (duration - note.destroy_time) / 20;

					if ((ix2 - cx) * (ix2 - cx) + (iy2 - cy) * (iy2 - cy) > 400)
					{
						continue;
					}

					if (r > 3 * note_r / 4)
					{
						continue;
					}
					else if (r >= 3 * note_r / 4 - 1)
					{
						buffer->put(ix2, iy2, ' ', NOTE_COLORS[string_n] | BACKGROUND_INTENSITY);
						continue;
					}
					buffer->put(ix2, iy2, ' ', NOTE_COLORS[string_n]);
				}
			}
		}
		else {
			for (auto ix = bx; ix <= ex; ++ix)
			{
				for (auto iy = by; iy <= ey; ++iy)
				{
					auto r = (int)sqrt((ix - cx) * (ix - cx) + 4 * (iy - cy) * (iy - cy));
					if (r > 3 * note_r / 4)
					{
						continue;
					}
					else if (r >= 3 * note_r / 4 - 1)
					{
						buffer->put(ix, iy, ' ', NOTE_COLORS[string_n] | BACKGROUND_INTENSITY);
						continue;
					}
					buffer->put(ix, iy, ' ', NOTE_COLORS[string_n]);
				}
			}
		}
	}

	void _draw_notes()
	{
		auto screen_height = buffer->screen_height();

		for (auto i = 0; i < notes->size(); ++i)
		{
			if (notes->at(i).time < ommision_time)
			{
				continue;
			}
			else if (notes->at(i).time <= duration + NOTE_START_TIME)
			{
				_put_note(notes->at(i));
			}
		}
	}

	void _draw_buttons(const std::array<bool, N_STRINGS> &button_states)
	{
		auto screen_width = buffer->screen_width();
		auto screen_height = buffer->screen_height();

		for (int stringi = 0; stringi < N_STRINGS; ++stringi)
		{
			auto cy = screen_height - 1;
			auto cx = _get_guitar_string_x(cy, stringi, screen_width, screen_height);

			auto note_r = (NOTE_MIN_R + cy * (NOTE_MAX_R - NOTE_MIN_R) / screen_height) / 2;
			auto bx = cx - note_r;
			auto by = cy - 2 * note_r / 5;
			auto ex = cx + note_r;
			auto ey = cy + 2 * note_r / 5;

			for (auto ix = bx; ix <= ex; ++ix)
			{
				for (auto iy = by; iy <= ey; ++iy)
				{
					auto r = (int)sqrt((ix - cx) * (ix - cx) + 4 * (iy - cy) * (iy - cy));
					if (r > 3 * note_r / 4)
					{
						continue;
					}
					else if (r >= 3 * note_r / 4 - 1)
					{
						buffer->put(ix, iy, ' ', NOTE_COLORS[stringi]);
						continue;
					}
					if (button_states[stringi])
					{
						buffer->put(ix, iy, ' ', NOTE_COLORS[stringi] | BACKGROUND_INTENSITY);
					}
				}
			}
		}
	}
};


void XOConsoleGameMapHero::draw(const std::shared_ptr<Console::Buffer>& buffer)
{

	auto now = std::chrono::system_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - _start_point);

	anim_helper_hero_map anim_proc
	{
		buffer,				// const std::shared_ptr<Console::Buffer>&buffer;
		_notes,				// const std::shared_ptr<NoteContainer> &notes;
		duration.count(),	// const std::chrono::system_clock::time_point &start_point;
		_omission_time		// long long ommision_time;
	};

	anim_proc._draw_guitar_neck(100 - _frame);
	anim_proc._draw_guitar_strings(100 - _frame);

	if (_play)
	{
		anim_proc._draw_notes();
	}
	anim_proc._draw_buttons(_button_states);
	_life_bar->draw(buffer);

	_progress_bar->progress(1.f - 1.f * duration.count() / _duration);
	_progress_bar->draw(buffer);

	_frame++;
}

void XOConsoleGameMapHero::draw_on(const std::shared_ptr<Console> &console)
{
	if (!_key_callback_binded)
	{
		console->key_down_event(_key_down_callback);
		console->key_up_event(_key_up_callback);
		_key_callback_binded = true;
	}
	console->clear_planes();

	auto chain_animation = std::dynamic_pointer_cast<AnimationChain>(console->animation());
	if (chain_animation)
	{
		auto game_animation = std::make_shared<GameHeroAnimation>();
		game_animation->start_frame(200);
		game_animation->speed(3);
		game_animation->center(0, -50);
		game_animation->player_symbol(_player_symbol);
		chain_animation->add(game_animation);
	}

	console->add_plane(shared_from_this());
}

void XOConsoleGameMapHero::_on_key(int key, bool down)
{
	for (auto &element : _elements)
	{
		if (down && element.id == std::to_string(key - '0') + " down")
		{
			element.callback();
			break;
		}
		else if (!down && element.id == std::to_string(key - '0') + " up")
		{
			element.callback();
			break;
		}
	}
}

void XOConsoleGameMapHero::feed(const std::shared_ptr<NoteContainer>& notes)
{
	_notes = notes;
}

void XOConsoleGameMapHero::player_symbol(xo::PlayerSymbol s)
{
	_player_symbol = s;
}

void XOConsoleGameMapHero::reset()
{
	_frame = 0;
	if (_notes)
	{
		_notes->clear();
	}
	_play = false;
}

void XOConsoleGameMapHero::progress(float p)
{
	_progress_bar->progress(p);
}

void XOConsoleGameMapHero::life(float p)
{
	_life_bar->progress(p);
}

void XOConsoleGameMapHero::button(int string_n, bool pressed)
{
	_button_states[string_n] = pressed;
}