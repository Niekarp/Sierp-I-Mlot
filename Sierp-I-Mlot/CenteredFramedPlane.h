#pragma once
#include "IConsolePlane.h"

struct CenteredFramedPlane :
	public virtual IConsolePlane
{
	CenteredFramedPlane(float position = 0);
	Position position() override;
	Position size() override;
	PlaneType type() override;
	void draw(const std::shared_ptr<Console::Buffer> &) override;

	void size(Position);
	void frame_color(WORD color);
	void fill_color(WORD color);
	void fill_char(char fill_char);
	void frame_chars(char left_chr, char top_chr, char right_chr, char bottom_chr);
	void corner_chars(char left_top_chr, char right_top_chr, char left_bottom_chr,
		char right_bottom_chr);

protected:
	char _fill_chr;
	char _left_chr, _top_chr, _right_chr, _bottom_chr;
	char _left_top_chr, _right_top_chr, _left_bottom_chr, _right_bottom_chr;
	Position _size;
	WORD _frame_color;
	WORD _fill_color;
	float _order_position;
	Position _position;
};

