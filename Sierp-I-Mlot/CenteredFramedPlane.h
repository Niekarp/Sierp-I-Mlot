#pragma once
#include "IConsolePlane.h"
#include "IFramedPlane.h"

struct CenteredFramedPlane :
	public virtual IConsolePlane,
	public xo::IFramedPlane
{
	CenteredFramedPlane(float position = 0);
	Position position() override;
	Position size() override;
	PlaneType type() override;
	void draw(const std::shared_ptr<Console::Buffer> &) override;

	void size(Position);
	void frame_color(WORD color);
	void fill_color(WORD color);
	void fill_char(char fill_char) override;
	void frame_chars(char left_chr, char top_chr, char right_chr, char bottom_chr) override;
	void corner_chars(char left_top_chr, char right_top_chr, char left_bottom_chr,
		char right_bottom_chr) override;
	void shadow(char chr, WORD color);

protected:
	char _fill_chr;
	char _left_chr, _top_chr, _right_chr, _bottom_chr;
	char _left_top_chr, _right_top_chr, _left_bottom_chr, _right_bottom_chr;
	char _shadow_chr;
	WORD _shadow_color;
	WORD _frame_color;
	WORD _fill_color;
	bool _shadow;
	Position _size;
	float _order_position;
	Position _position;
};

