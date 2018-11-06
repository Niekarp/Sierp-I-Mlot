#include "pch.h"
#include "XOConsoleGameMap.h"
#include "XOConsoleMapPlane.h"


namespace xo
{
	void XOConsoleGameMap::register_element(const XOViewElement &view_element)
	{
		_elements.push_back(view_element);
	}

	void XOConsoleGameMap::draw_on(const std::shared_ptr<Console>& output)
	{
		output->clear_planes();

		auto game_plane = std::make_shared<XOConsoleMapPlane>();
		game_plane->size({50, 30});
		game_plane->color(BACKGROUND_RED | BACKGROUND_INTENSITY);

		output->add_plane(game_plane);
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
		// tutaj pawo³ czaruje
	}
}