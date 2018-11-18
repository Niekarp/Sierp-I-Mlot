#include "pch.h"
#include "XOConsoleGameMapXO.h"
#include "XOConsoleMapPlane.h"
#include "AnimationChain.h"
#include "IntroMenuAnimation.h"
#include "GameXOAnimation.h"

using namespace xo;

XOConsoleGameMapXO::XOConsoleGameMapXO() :
	_field_x_path(""),
	_field_o_path("")
{
}

void XOConsoleGameMapXO::draw_on(const std::shared_ptr<Console>& output)
{
	_map_plane = std::make_shared<XOConsoleMapPlane>();
	_map_plane->size({ conf::GAME_MAP_XO_SIZE_X, conf::GAME_MAP_XO_SIZE_Y });
	_map_plane->foreground(BACKGROUND_RED | BACKGROUND_INTENSITY);
	_map_plane->background(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
	std::vector<std::shared_ptr<FileImagePlane>> images{
		std::make_shared<FileImagePlane>(_field_x_path.c_str()),
		std::make_shared<FileImagePlane>(_field_o_path.c_str()) };
	images[0]->color(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
	images[1]->color(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
	_map_plane->images(images);
	using namespace std::placeholders;
	_map_plane->click(std::bind(&XOConsoleGameMapXO::_on_click, this, _1, _2));

	output->clear_planes();

	auto animation = std::dynamic_pointer_cast<AnimationChain>(output->animation());
	if (animation)
	{
		auto menu_background_animation = std::make_shared<GameXOAnimation>(600, 1);
		menu_background_animation->speed(2);
		animation->add(menu_background_animation);

		auto menu_animation = std::make_shared<IntroMenuAnimation>(
			IConsolePlane::Position{ conf::GAME_MAP_XO_SIZE_X, conf::GAME_MAP_XO_SIZE_Y });
		menu_animation->fill(176, FOREGROUND_RED | FOREGROUND_INTENSITY);
		animation->add(menu_animation);

		animation->on_end([this, output](auto frame)
		{
			output->add_clickable_plane(_map_plane);
		});
	}
	else 
	{		
		output->add_clickable_plane(_map_plane);
	}
}

void xo::XOConsoleGameMapXO::field_x(const std::string & path)
{
	_field_x_path = path;
}

void xo::XOConsoleGameMapXO::field_o(const std::string & path)
{
	_field_o_path = path;
}

size_t XOConsoleGameMapXO::width()
{
	return _width;
}

void XOConsoleGameMapXO::width(size_t new_width)
{
	_width = new_width;
}

size_t XOConsoleGameMapXO::height()
{
	return _height;
}

void XOConsoleGameMapXO::height(size_t new_height)
{
	_height = new_height;
}

void XOConsoleGameMapXO::put(PlayerSymbol symbol, size_t x, size_t y)
{
	_map_plane->put(x, y, (int)symbol);
}

void XOConsoleGameMapXO::clear_fields()
{

}

void XOConsoleGameMapXO::_on_click(int col, int row)
{
	auto id = std::to_string(col) + " " + std::to_string(row);
	element_call(id);
}