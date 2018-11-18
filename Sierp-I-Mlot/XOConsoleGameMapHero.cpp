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
	_frame(0)
{
	_note_image = std::make_shared<FileImagePlane>("resources/note.txt");
	_note_image->color(FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
}


XOConsoleGameMapHero::~XOConsoleGameMapHero()
{
}

void XOConsoleGameMapHero::register_element(const XOViewElement &element)
{
	_elements.push_back(element);
}

void XOConsoleGameMapHero::put(const XOConsoleGameMapHero::Note &note)
{
	_notes.push_back(note);
	//_notes.sort([](auto a, auto b) { return a.time < b.time; });
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

inline static void _draw_guitar_neck(const std::shared_ptr<Console::Buffer>& buffer, int by)
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

inline static void _draw_guitar_strings(const std::shared_ptr<Console::Buffer>& buffer, int by)
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

inline static void _put_note(const std::shared_ptr<Console::Buffer>& buffer,
	int string_n, int offset)
{
	auto screen_width = buffer->screen_width();
	auto screen_height = buffer->screen_height();

	auto cy = screen_height - 1 - offset;
	auto cx = _get_guitar_string_x(cy, string_n, screen_width, screen_height);

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
				buffer->put(ix, iy, ' ', NOTE_COLORS[string_n] | BACKGROUND_INTENSITY);
				continue;
			}
			buffer->put(ix, iy, ' ', NOTE_COLORS[string_n]);
		}
	}
}

inline static void _draw_notes(const std::shared_ptr<Console::Buffer>& buffer,
	const std::vector<XOConsoleGameMapHero::Note> &_notes, 
	const std::chrono::system_clock::time_point &_start_point)
{
	auto duration = std::chrono::system_clock::now() - _start_point;
	auto elapse = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	auto screen_height = buffer->screen_height();

	for (auto &note : _notes)
	{
		if (note.time < elapse - NOTE_START_TIME / 8)
		{
			continue;
		}
		else if (note.time <= elapse + NOTE_START_TIME)
		{
			_put_note(buffer, note.tone, (note.time - elapse) * screen_height / NOTE_START_TIME);
		}
	}
}

inline static void _draw_buttons(const std::shared_ptr<Console::Buffer>& buffer,
	const std::array<bool, N_STRINGS> &button_states)
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

void XOConsoleGameMapHero::draw(const std::shared_ptr<Console::Buffer>& buffer)
{
	_draw_guitar_neck(buffer, 100 - _frame);
	_draw_guitar_strings(buffer, 100 - _frame);
	_draw_notes(buffer, _notes, _start_point);
	_draw_buttons(buffer, _button_states);

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

void XOConsoleGameMapHero::button(int string_n, bool pressed)
{
	_button_states[string_n] = pressed;
}

void XOConsoleGameMapHero::start(long long wait)
{
	_start_point = std::chrono::system_clock::now() + std::chrono::milliseconds(wait);
}
