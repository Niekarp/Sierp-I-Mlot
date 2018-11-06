#include "pch.h"
#include "XOConsoleMapPlane.h"

XOConsoleMapPlane::XOConsoleMapPlane(int cols, int rows) :
	_cols(cols),
	_rows(rows),
	_size({}),
	_position({}),
	_color(0)
{
}

XOConsoleMapPlane::Position XOConsoleMapPlane::position()
{
	return { 0, 0 };
}

XOConsoleMapPlane::Position XOConsoleMapPlane::size()
{
	return _size;
}

XOConsoleMapPlane::PlaneType XOConsoleMapPlane::type()
{
	return PlaneType::CENTERED;
}

void XOConsoleMapPlane::draw(const std::shared_ptr<Console::Buffer>& buffer)
{
	auto cx = buffer->screen_width() / 2;
	auto cy = buffer->screen_height() / 2;
	auto bx = cx - _size.x / 2;
	auto by = cy - _size.y / 2;
	auto ex = cx + _size.x / 2;
	auto ey = cy + _size.y / 2;
	auto col_w = (ex - bx) / _cols;
	auto row_h = (ey - by) / _rows;

	bx = bx < 0 ? 0 : bx;
	by = by < 0 ? 0 : by;
	ex = buffer->screen_width() < ex ? buffer->screen_width() : ex;
	ey = buffer->screen_height() < ey ? buffer->screen_height() : ey;

	_position.x = cx;
	_position.y = cy;

	for (int i = 0; i < _cols + 1; ++i)
	{
		for (int j = by; j < ey - 1; ++j)
		{
			buffer->put(bx + i * col_w, j, ' ', _color);
		}
	}

	for (int i = bx; i < ex - 1; ++i)
	{
		for (int j = 0; j < _rows + 1; ++j)
		{
			buffer->put(i, by + j * row_h, ' ', _color);
		}
	}
}

void XOConsoleMapPlane::click(IConsolePlane::Position position, DWORD btn, DWORD flag)
{
}

void XOConsoleMapPlane::click_release()
{
}

void XOConsoleMapPlane::size(const Position &size)
{
	_size = size;
}

void XOConsoleMapPlane::color(WORD color)
{
	_color = color;
}
