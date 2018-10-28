#include "stdafx.h"
#include "CenteredFramedPlane.h"


CenteredFramedPlane::CenteredFramedPlane(int position) :
	_fill_chr(' '),
	/*_left_chr('\xb3'),
	_top_chr('\xc4'),
	_right_chr('\xb3'),
	_bottom_chr('\xc4'),
	_left_top_chr('\xda'),
	_right_top_chr('\xbf'),
	_left_bottom_chr('\xc0'),
	_right_bottom_chr('\xd9'),*/
	_left_chr('\xb1'),
	_top_chr('\xb1'),
	_right_chr('\xb1'),
	_bottom_chr('\xb1'),
	_left_top_chr('\xb1'),
	_right_top_chr('\xb1'),
	_left_bottom_chr('\xb1'),
	_right_bottom_chr('\xb1'),
	_size({ 1, 1 }),
	_order_position(position),
	_position({0, 0})
{
}

CenteredFramedPlane::Position CenteredFramedPlane::position()
{
	return _position;
}

CenteredFramedPlane::Position CenteredFramedPlane::size()
{
	return _size;
}

CenteredFramedPlane::PlaneType CenteredFramedPlane::type()
{
	return PlaneType::CENTERED;
}

void CenteredFramedPlane::draw(const std::shared_ptr<Console::Buffer>& buffer)
{
	auto cx = buffer->screen_width() / 2;
	auto cy = buffer->screen_height() / 2 + _order_position * (_size.y + 1);
	auto bx = cx - _size.x / 2;
	auto by = cy - _size.y / 2;
	auto ex = cx + _size.x / 2;
	auto ey = cy + _size.y / 2;

	bx = bx < 0 ? 0 : bx;
	by = by < 0 ? 0 : by;
	ex = buffer->screen_width() < ex ? buffer->screen_width() : ex;
	ey = buffer->screen_height() < ey? buffer->screen_height() : ey;

	_position.x = cx;
	_position.y = cy;

	// corners
	buffer->put(bx, by, _left_top_chr, _frame_color);
	buffer->put(ex - 1, by, _right_top_chr, _frame_color);
	buffer->put(bx, ey - 1, _left_bottom_chr, _frame_color);
	buffer->put(ex - 1, ey - 1, _right_bottom_chr, _frame_color);

	// left
	for (auto j = by + 1; j < ey - 1; ++j)
	{
		buffer->put(bx, j, _left_chr, _frame_color);
	}

	// top
	for (auto i = bx + 1; i < ex - 1; ++i)
	{
		buffer->put(i, by, _top_chr, _frame_color);
	}

	// right
	for (auto j = by + 1; j < ey - 1; ++j)
	{
		buffer->put(ex - 1, j, _right_chr, _frame_color);
	}

	// bottom
	for (auto i = bx + 1; i < ex - 1; ++i)
	{
		buffer->put(i, ey - 1, _bottom_chr, _frame_color);
	}

	// fill
	for (auto i = bx + 1; i < ex - 1; ++i)
	{
		for (auto j = by + 1; j < ey - 1; ++j)
		{
			buffer->put(i, j, _fill_chr, _fill_color);
		}
	}
}

void CenteredFramedPlane::size(Position size)
{
	_size = size;
}

void CenteredFramedPlane::frame_color(WORD color)
{
	_frame_color = color;
}

void CenteredFramedPlane::fill_color(WORD color)
{
	_fill_color = color;
}

void CenteredFramedPlane::fill_char(char fill_char)
{
	_fill_chr = fill_char;
}

void CenteredFramedPlane::frame_chars(char left_chr, char top_chr, char right_chr, char bottom_chr)
{
	_left_chr = left_chr;
	_top_chr = top_chr;
	_right_chr = right_chr;
	_bottom_chr = bottom_chr;
}

void CenteredFramedPlane::corner_chars(char left_top_chr, char right_top_chr, char left_bottom_chr, char right_bottom_chr)
{
	_left_top_chr = left_top_chr;
	_right_top_chr = right_top_chr;
	_left_bottom_chr = left_bottom_chr;
	_right_bottom_chr = right_bottom_chr;
}
