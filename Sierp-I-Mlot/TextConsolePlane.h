#pragma once
#include "IConsolePlane.h"

struct TextConsolePlane :
	public IConsolePlane
{
	TextConsolePlane();
	
	Position position() override;
	Position size() override;
	PlaneType type() override;
	void draw(const std::shared_ptr<Console::Buffer> &) override;

	void font_size(const Position &new_size);
	void text(const std::string &str);
	void load_font(const std::string &directory);
	void position(const Position &);

	void foreground(char chr, WORD color);
	void background(char chr, WORD color);

private:
	void _load_font_char(const std::string &directory, char chr);

	struct Letter
	{
		int width;
		std::vector<char> data;
	};
	Position _font_size;
	std::string _text;
	std::map<char, Letter> _letters;
	Position _position;
	Position _size;
	WORD _foreground_color;
	WORD _background_color;
	char _foreground_chr;
	char _background_chr;
};

