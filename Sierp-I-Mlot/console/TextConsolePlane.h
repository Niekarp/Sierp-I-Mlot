#pragma once
#include "IConsolePlane.h"

struct TextConsolePlane :
	public IConsolePlane
{
	TextConsolePlane(const char *text, const char *font_directory);
	
	Position position() override;
	Position size() override;
	PlaneType type() override;
	void draw(const std::shared_ptr<Console::Buffer> &) override;

	void text(const char *str);
	void load_font(const char *directory);
	void position(Position);

	void foreground(char chr, WORD color);
	void background(char chr, WORD color);

private:
	static const int LETTER_SIZE = 12;
	const char *_text;
	std::map<char, std::array<char, LETTER_SIZE>> _letters;
	const int _letter_width = 3;
	Position _position;
	WORD _foreground_color;
	WORD _background_color;
	char _foreground_chr;
	char _background_chr;
};

