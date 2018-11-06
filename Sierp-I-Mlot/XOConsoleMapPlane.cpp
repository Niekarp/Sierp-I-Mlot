#include "pch.h"
#include "XOConsoleMapPlane.h"

XOConsoleMapPlane::XOConsoleMapPlane(int cols, int rows) :
	_cols(cols),
	_rows(rows),
	_size({}),
	_position({}),
	_color_background(0),
	_color_foreground(0)
{
	_fields.resize(cols * rows);
	_x_plane = std::make_shared<CenteredFramedPlane>();
}

XOConsoleMapPlane::Position XOConsoleMapPlane::position()
{
	return _position;
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

	bx = bx < 0 ? 0 : bx;
	by = by < 0 ? 0 : by;
	ex = buffer->screen_width() < ex ? buffer->screen_width() : ex;
	ey = buffer->screen_height() < ey ? buffer->screen_height() : ey;

	_position.x = cx;
	_position.y = cy;

	// background
	for (int i = bx + 1; i < ex; ++i)
	{
		for (int j = by + 1; j < ey; ++j)
		{
			buffer->put(i, j, ' ', _color_background);
		}
	}

	// foreground
	for (int i = 0; i < _cols + 1; ++i)
	{
		for (int j = by; j <= ey; ++j)
		{
			buffer->put(bx + i * (ex - bx) / _cols, j, ' ', _color_foreground);
		}
	}

	for (int i = bx; i <= ex; ++i)
	{
		for (int j = 0; j < _rows + 1; ++j)
		{
			buffer->put(i, by + j * (ey - by) / _rows, ' ', _color_foreground);
		}
	}

	// Xs and Os
	for(int i = 0; i < _cols; ++i)
	{
		for (int j = 0; j < _rows; ++j)
		{

		}
	}
}

void XOConsoleMapPlane::click(IConsolePlane::Position position, DWORD btn, DWORD flag)
{
	auto rel_pos_x = position.x - (_position.x - _size.x / 2);
	auto rel_pos_y = position.y - (_position.y - _size.y / 2);
	
	auto col = rel_pos_x / (_size.x / _cols);
	auto row = rel_pos_y / (_size.y / _rows);

	put(col, row, 1);
}

void XOConsoleMapPlane::click_release()
{
}

void XOConsoleMapPlane::size(const Position &size)
{
	_size = size;
}

void XOConsoleMapPlane::foreground(WORD color)
{
	_color_foreground = color;
}

void XOConsoleMapPlane::background(WORD color)
{
	_color_background = color;
}

void XOConsoleMapPlane::put(int col, int row, int f)
{
	if (col >= _cols) 
	{
		return;
	}
	if (row >= _rows)
	{
		return;
	}
	
	_fields[row * _cols + col] = f;
}

