#include "pch.h"
#include "FileImagePlane.h"

FileImagePlane::FileImagePlane(const std::string &filename) :
	_pattern_size({ 1, 1 }),
	_pattern_pos({}),
	_real_pattern_pos{},
	_color(0),
	_gravity(Gravity::DEFAULT),
	_clicked(false),
	_real_pattern_size{}
{
	load(filename);
}

FileImagePlane::Position FileImagePlane::position()
{
	if (_gravity == Gravity::DEFAULT)
	{
		return _pattern_pos;
	}
	else if (_gravity == Gravity::LEFTDOWN)
	{
		return { _real_pattern_pos.x + _real_pattern_size.x / 2, _real_pattern_pos.y + _real_pattern_size.y / 2 };
	}
}

FileImagePlane::Position FileImagePlane::size()
{
	if (_gravity == Gravity::DEFAULT)
	{
		return _pattern_size;
	}
	else if (_gravity == Gravity::LEFTDOWN)
	{
		return _real_pattern_size;
	}
}

FileImagePlane::PlaneType FileImagePlane::type()
{
	return PlaneType::CENTERED;
}

void FileImagePlane::draw(const std::shared_ptr<Console::Buffer>& buffer)
{
	auto width = _pattern_size.x;
	auto height = _pattern_size.y;
	auto screen_width = buffer->screen_width();
	auto screen_height = buffer->screen_height();

	int bx = 0;
	int by = 0;
	int ex = 0;
	int ey = 0;

	if (_gravity == Gravity::DEFAULT)
	{
		bx = _pattern_pos.x;
		by = _pattern_pos.y;
		ex = bx + _pattern_size.x - 1;
		ey = by + _pattern_size.y - 1;
	}
	else if (_gravity == Gravity::LEFTDOWN)
	{		
		bx = _pattern_pos.x;
		by = screen_height - _pattern_size.y - _pattern_pos.y;
		ex = bx + _pattern_size.x - 1;
		ey = by + _pattern_size.y - 1;

		_real_pattern_pos.x = bx;
		_real_pattern_pos.y = by;
		_real_pattern_size.x = ex - bx;
		_real_pattern_size.y = ey - by;
	}

	for (int ix = bx; ix <= ex; ++ix)
	{
		for (int iy = by; iy <= ey; ++iy)
		{
			auto chr = _pattern[(iy - by) * _pattern_size.x + (ix - bx)];
			if (chr != ' ')
			{
				chr = chr == '#' ? ' ' : chr;
				auto color = _color;
				if (_clicked)
				{
					color ^= BACKGROUND_INTENSITY;
				}
				buffer->put(ix , iy, chr, color);
			}
		}
	}

}

void FileImagePlane::position(Position pos)
{
	_pattern_pos = pos;
}

void FileImagePlane::load(const std::string &filename)
{
	std::fstream input_file(filename, std::ios::in);
	if (!input_file)
	{
		throw std::invalid_argument("!input_file");
	}

	input_file >> _pattern_size.x;

	char chr;
	while (input_file >> std::noskipws >> chr)
	{
		if (chr == '\n') continue;
		_pattern.push_back(chr);
	}

	_pattern_size.y = _pattern.size() / _pattern_size.x;
	if (_pattern_size.y * _pattern_size.x != _pattern.size())
	{
		throw std::invalid_argument("_pattern_height * _pattern_width != _pattern.size()");
	}
}

void FileImagePlane::color(WORD color)
{
	_color = color;
}

void FileImagePlane::gravity(Gravity g)
{
	_gravity = g;
}

void FileImagePlane::click_event(const std::function<void()> &callback)
{
	_click_callback = callback;
}

void FileImagePlane::click(IConsolePlane::Position position, DWORD btn, DWORD flag)
{
	_clicked = true;
}

void FileImagePlane::click_release()
{
	_clicked = false;
	if (_click_callback)
	{
		_click_callback();
	}
}