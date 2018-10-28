#include "pch.h"
#include "TextConsolePlane.h"

TextConsolePlane::TextConsolePlane(const char * str, const char *font_directory) :
	_foreground_color(0),
	_background_color(0),
	_position({0, 0})
{
	text(str);
	load_font(font_directory);
}

TextConsolePlane::Position TextConsolePlane::position()
{
	return _position;
}

void TextConsolePlane::position(TextConsolePlane::Position position)
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
	return { (int)strlen(_text) * (_letter_width + 1), 4 };
}

TextConsolePlane::PlaneType TextConsolePlane::type()
{
	return PlaneType::POSITIONABLE;
}

void TextConsolePlane::draw(const std::shared_ptr<Console::Buffer>& buffer)
{
	auto text_len = strlen(_text);

	for (int i = 0; i < text_len; ++i)
	{
		auto letter = _letters[_text[i]];

		auto bx = _position.x + i * (_letter_width + 1);
		auto by = _position.y;

		for (int j = 0; j < letter.size() / _letter_width; ++j)
		{
			for (int k = 0; k < _letter_width; ++k)
			{
				if (letter[j * _letter_width + k] - '0')
				{
					buffer->put(bx + k, by + j, _foreground_chr, _foreground_color);
				}
				else
				{
					buffer->put(bx + k, by + j, _background_chr, _background_color);
				}
			}
		}
	}
}

void TextConsolePlane::text(const char * str)
{
	_text = str;
}

void TextConsolePlane::load_font(const char * directory)
{
	char letter[LETTER_SIZE];
	for (auto current_chr = 'a'; current_chr <= 'z'; ++current_chr)
	{
		std::fstream letter_file(std::string(directory) + "/" + current_chr + std::string(".txt"), std::ios::in);
		int i = 0;
		while (letter_file >> letter[i++] && i < sizeof(letter));
		_letters[current_chr];
		std::copy(std::begin(letter), std::end(letter), std::begin(_letters[current_chr]));
	}
}
