#include "pch.h"
#include "XOConsoleGameMap.h"
#include "XOConsoleMapPlane.h"


namespace xo
{
	XOConsoleGameMap::XOConsoleGameMap()
	{
	}
	
	void XOConsoleGameMap::register_element(const XOViewElement &view_element)
	{
		_elements.push_back(view_element);
	}

	void XOConsoleGameMap::draw_on(const std::shared_ptr<Console>& output)
	{
		_map_plane = std::make_shared<XOConsoleMapPlane>();
		_map_plane->size({ 60, 36 });
		_map_plane->foreground(BACKGROUND_RED | BACKGROUND_INTENSITY);
		_map_plane->background(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
		std::vector<std::shared_ptr<FileImagePlane>> images{
			std::make_shared<FileImagePlane>("resources/x_field.txt"),
			std::make_shared<FileImagePlane>("resources/o_field.txt") };
		images[0]->color(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
		images[1]->color(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
		_map_plane->images(images);
		using namespace std::placeholders;
		_map_plane->click(std::bind(&XOConsoleGameMap::_on_click, this, _1, _2));

		output->clear_planes();
		output->add_clickable_plane(_map_plane);
	}

	size_t XOConsoleGameMap::width()
	{
		return _width;
	}

	void XOConsoleGameMap::width(size_t new_width)
	{
		_width = new_width;
	}

	size_t XOConsoleGameMap::height()
	{
		return _height;
	}

	void XOConsoleGameMap::height(size_t new_height)
	{
		_height = new_height;
	}

	void XOConsoleGameMap::put(PlayerSymbol symbol, size_t x, size_t y)
	{
		_map_plane->put(x, y, (int)symbol);
	}

	void XOConsoleGameMap::clear_fields()
	{

	}

	void XOConsoleGameMap::_on_click(int col, int row)
	{
		auto id = std::to_string(col) + " " + std::to_string(row);
		for (auto &element : _elements)
		{
			if (element.id == id)
			{
				element.callback();
			}
		}
	}
}