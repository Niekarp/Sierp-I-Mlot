#include "pch.h"
#include "XOConsoleMapPlane.h"

XOConsoleMapPlane::XOConsoleMapPlane(int cols, int rows) :
	_cols(cols),
	_rows(rows),
	_size({}),
	_position({}),
	_color_background(0),
	_color_foreground(0),
	_frame(0)
{
	colrow(cols, rows);
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
	auto row_h = 1. * (ey - by) / _rows;
	auto col_w = 1. * (ex - bx) / _cols;

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
			buffer->put(bx + i * col_w, j, ' ', _color_foreground);
		}
	}

	for (int i = bx; i <= ex; ++i)
	{
		for (int j = 0; j < _rows + 1; ++j)
		{
			buffer->put(i, by + j * row_h, ' ', _color_foreground);
		}
	}

	// Xs and Os
	for(int col = 0; col < _cols; ++col)
	{
		for (int row = 0; row < _rows; ++row)
		{
			if (_fields[row * _cols + col] != xo::PlayerSymbol::none)
			{
				auto &field_image = _images[(int)_fields[row * _cols + col]];
				field_image->position({ 
					bx + (int)((col + 0.5) * col_w - field_image->size().x / 2),
					by + (int)((row + 0.5) * row_h) - field_image->size().y / 2 });
				field_image->draw(buffer);
			}		
		}
	}

	// Xs and Os select
	if (_frame / xo::conf::GAME_XO_SELECT_FLASHING_SPEED % 2)
	{
		for (int col = 0; col < _cols; ++col)
		{
			for (int row = 0; row < _rows; ++row)
			{
				if (_selected_fields[row * _cols + col] != xo::PlayerSymbol::none)
				{
					auto &field_image = _images[(int)_selected_fields[row * _cols + col]];
					field_image->position({
						bx + (int)((col + 0.5) * col_w - field_image->size().x / 2),
						by + (int)((row + 0.5) * row_h) - field_image->size().y / 2 });
					field_image->draw(buffer);
				}
			}
		}
	}

	_frame += 1;
}

void XOConsoleMapPlane::click(IConsolePlane::Position position, DWORD btn, DWORD flag)
{
	auto rel_pos_x = position.x - (_position.x - _size.x / 2);
	auto rel_pos_y = position.y - (_position.y - _size.y / 2);
	
	auto col = rel_pos_x / (_size.x / _cols);
	auto row = rel_pos_y / (_size.y / _rows);

	if (_callback_click)
	{
		_callback_click(col, row);
	}
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

void XOConsoleMapPlane::put(int col, int row, xo::PlayerSymbol f)
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

void XOConsoleMapPlane::select(int col, int row, xo::PlayerSymbol f)
{
	if (col >= _cols)
	{
		return;
	}
	if (row >= _rows)
	{
		return;
	}

	_selected_fields[row * _cols + col] = f;
}

void XOConsoleMapPlane::images(const std::vector<std::shared_ptr<FileImagePlane>>& images)
{
	_images = images;
}

void XOConsoleMapPlane::click(std::function<void(int, int)> callback)
{
	_callback_click = callback;
}

void XOConsoleMapPlane::colrow(int cols, int rows)
{
	_cols = cols;
	_rows = rows;

	_fields.resize(cols * rows);
	std::fill(_fields.begin(), _fields.end(), xo::PlayerSymbol::none);
	_selected_fields.resize(cols * rows);
	std::fill(_selected_fields.begin(), _selected_fields.end(), xo::PlayerSymbol::none);
}