#include "pch.h"
#include "FileImagePlane.h"

FileImagePlane::FileImagePlane(const std::string &filename) :
	_pattern_size({ 1, 1 }),
	_pattern_pos({}),
	_color(0)
{
	load(filename);
}

FileImagePlane::Position FileImagePlane::position()
{
	return _pattern_pos;
}

FileImagePlane::Position FileImagePlane::size()
{
	return _pattern_size;
}

FileImagePlane::PlaneType FileImagePlane::type()
{
	return PlaneType::POSITIONABLE;
}

void FileImagePlane::draw(const std::shared_ptr<Console::Buffer>& buffer)
{
	auto width = _pattern_size.x;
	auto height = _pattern_size.y;
	auto screen_width = buffer->screen_width();
	auto screen_height = buffer->screen_height();

	for (int i = 0; i < width; ++i)
	{
		if (i + _pattern_pos.x < 0 || i + _pattern_pos.x > screen_width - 1)
		{
			continue;
		}
		for (int j = 0; j < height; ++j)
		{
			if (j + _pattern_pos.y < 0 || j + _pattern_pos.y > screen_height - 1)
			{
				continue;
			}
			auto chr = _pattern[j * _pattern_size.x + i];
			if (chr != ' ')
			{
				chr = chr == '#' ? ' ' : chr;
				buffer->put(i + _pattern_pos.x, j + _pattern_pos.y, chr, _color);
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
