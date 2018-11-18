#include "pch.h"
#include "TextConsolePlane.h"

TextConsolePlane::TextConsolePlane() :
	_foreground_color(0),
	_background_color(0),
	_position({0, 0}),
	_font_size({0, 0}),
	_size({0, 0})
{
}

TextConsolePlane::Position TextConsolePlane::position()
{
	return _position;
}

void TextConsolePlane::position(const TextConsolePlane::Position &position)
{
	_position = position;
}

void TextConsolePlane::foreground(char chr, WORD color)
{
	_foreground_chr = chr;
	_foreground_color = color;
}

void TextConsolePlane::background(char chr, WORD color)
{
	_background_chr = chr;
	_background_color = color;
}

TextConsolePlane::Position TextConsolePlane::size()
{
	if (_font_size.x == 0)
	{
		return _size;
	}

	return { (int)_text.size() * (_font_size.x + 1), _font_size.y };
}

TextConsolePlane::PlaneType TextConsolePlane::type()
{
	return PlaneType::POSITIONABLE;
}

void TextConsolePlane::draw(const std::shared_ptr<Console::Buffer>& buffer)
{
	auto bx = _position.x;
	auto by = _position.y;
	auto offsetx = 0;

	for (int i = 0; i < _text.size(); ++i)
	{
		if (_text[i] == ' ')
		{
			continue;
		}

		auto letter = _letters[_text[i]];

		for (int iy = 0; iy < letter.data.size() / letter.width; ++iy)
		{
			for (int ix = 0; ix < letter.width; ++ix)
			{
				if (_foreground_chr == 0)
				{
					if (letter.data[iy * letter.width + ix] - '0')
					{
						buffer->put(bx + offsetx + ix, by + iy,
							letter.data[iy * letter.width + ix], _foreground_color);
					}
				}
				else
				{
					if (letter.data[iy * letter.width + ix] - '0')
					{
						buffer->put(bx + offsetx + ix, by + iy,
							_foreground_chr, _foreground_color);
					}
					else
					{
						buffer->put(bx + offsetx + ix, by + iy,
							_background_chr, _background_color);
					}
				}
			}
		}

		offsetx += letter.width + 1;
	}
}

void TextConsolePlane::font_size(const Position & new_size)
{
	_font_size = new_size;
}

void TextConsolePlane::text(const std::string & str)
{
	if (_font_size.x == 0)
	{
		_size.x = 0;
		_size.y = 0;
		for (auto chr : str)
		{
			_size.x += _letters[chr].width + 1;
			auto h = _letters[chr].data.size() / _letters[chr].width;
			if (h > _size.y)
			{
				_size.y = h;
			}
		}
	}
	_text = str;
}

void TextConsolePlane::load_font(const std::string & directory)
{
	for (auto current_chr = 'a'; current_chr <= 'z'; ++current_chr)
	{
		_load_font_char(directory, current_chr);
	}
	for (auto current_chr = '0'; current_chr <= '9'; ++current_chr)
	{
		_load_font_char(directory, current_chr);
	}
	_load_font_char(directory, '_');
}

void TextConsolePlane::_load_font_char(const std::string &directory, 
	char current_chr)
{
	std::fstream letter_file(std::string(directory) + "/" 
		+ current_chr + std::string(".txt"), std::ios::in);

	auto &letter = _letters[current_chr];

	// dynamic font width
	if (_font_size.x == 0)
	{
		letter_file >> letter.width;
		letter.data.reserve(letter.width);
	}
	else
	{
		letter.width = _font_size.x;
	}

	char chr;
	while (letter_file >> std::noskipws >> chr)
	{
		if (chr == '\n') continue;
		letter.data.push_back(chr);
	}

	if (letter.data.size() % letter.width != 0)
	{
		throw std::invalid_argument("_pattern_height * _pattern_width != _pattern.size()");
	}
}
