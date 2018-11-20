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
	_map_plane = std::make_shared<XOConsoleMapPlane>();
	_map_plane->size({ conf::GAME_MAP_XO_SIZE_X, conf::GAME_MAP_XO_SIZE_Y });
	_map_plane->foreground(BACKGROUND_RED | BACKGROUND_INTENSITY);
	_map_plane->background(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);

	using namespace std::placeholders;
	_map_plane->click(std::bind(&XOConsoleGameMapXO::_on_click, this, _1, _2));

	_back_button = std::make_shared<FileImagePlane>(conf::GAME_XO_BACK_IMAGE);
	_back_button->color(FOREGROUND_RED | FOREGROUND_INTENSITY);
	_back_button->gravity(FileImagePlane::Gravity::LEFTDOWN);
	_back_button->position({ 0, 3 });

	_back_button->click_event([this] {
		element_call("back");
	});
}

void XOConsoleGameMapXO::draw_on(const std::shared_ptr<Console>& output)
{
	std::vector<std::shared_ptr<FileImagePlane>> images{
	std::make_shared<FileImagePlane>(_field_x_path.c_str()),
	std::make_shared<FileImagePlane>(_field_o_path.c_str()) };
	images[0]->color(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
	images[1]->color(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
	_map_plane->images(images);

	output->clear_planes();

	auto animation = std::dynamic_pointer_cast<AnimationChain>(output->animation());
	if (animation)
	{
		auto menu_background_animation = std::make_shared<GameXOAnimation>(600);
		menu_background_animation->speed(2);
		menu_background_animation->player_symbol(_player_symbol);
		animation->add(menu_background_animation);

		auto menu_animation = std::make_shared<IntroMenuAnimation>(
			IConsolePlane::Position{ conf::GAME_MAP_XO_SIZE_X, conf::GAME_MAP_XO_SIZE_Y });
		menu_animation->fill(176, FOREGROUND_RED | FOREGROUND_INTENSITY);
		animation->add(menu_animation);

		animation->on_end([this, output](auto frame)
		{
			output->add_clickable_plane(_map_plane);
			output->add_clickable_plane(_back_button);
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

void xo::XOConsoleGameMapXO::player_symbol(PlayerSymbol s)
{
	_player_symbol = s;
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
	_map_plane->put(x, y, symbol);
}

void XOConsoleGameMapXO::clear_fields()
{
	for (int ix = 0; ix < _width; ++ix)
	{
		for (auto iy = 0; iy < _height; ++iy)
		{
			_map_plane->put(ix, iy, PlayerSymbol::none);
		}
	}
}

void XOConsoleGameMapXO::_on_click(int col, int row)
{
	auto id = std::to_string(col) + " " + std::to_string(row);
	element_call(id);
}

void xo::XOConsoleGameMapXO::clear_selection()
{
	for (int ix = 0; ix < _width; ++ix)
	{
		for (auto iy = 0; iy < _height; ++iy)
		{
			_map_plane->select(ix, iy, PlayerSymbol::none);
		}
	}
}

void xo::XOConsoleGameMapXO::select(PlayerSymbol symbol, size_t x, size_t y)
{
	_map_plane->select(x, y, symbol);
}
