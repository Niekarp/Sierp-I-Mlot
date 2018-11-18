#include "pch.h"
#include "XOActionLogic.h"
#include "ChartReader.h"


using namespace xo;

XOActionLogic::XOActionLogic(std::shared_ptr<XOIOutput> &output, XOGameLogic &game_logic) :
	_game_logic(game_logic),
	_scale(1.f),
	_next_x(0),
	_next_y(0)
{
	_output = output;

	_main_menu = _output->create_menu();
	_main_menu->register_element({ "1 play button", "play", [this] { this->main_menu_play(); } });
	_main_menu->register_element({ "2 settings button", "settings", [this] { this->main_menu_settings(); } });
	_main_menu->register_element({ "3 exit button", "exit", [this] { this->main_menu_exit(); } });

	_game_map_xo = _output->create_game_map_xo();
	_game_map_xo->width(3);
	_game_map_xo->height(3);
	for (int i = 0; i < 9; ++i)
	{
		auto col = i % 3;
		auto row = i / 3;
		std::string coords = std::to_string(col) + " " + std::to_string(row);
		_game_map_xo->register_element({ coords, "", [this, col, row]
		{
			_next_x = col;
			_next_y = row;
			_output->show(_ready_message);
			_music_player->stop();
		} });
	}

	_game_map_hero = _output->create_game_map_hero();
	_game_map_hero->register_element({ "1 down", "", [this] { game_hero_key(1, true); } });
	_game_map_hero->register_element({ "2 down", "", [this] { game_hero_key(2, true); } });
	_game_map_hero->register_element({ "3 down", "", [this] { game_hero_key(3, true); } });
	_game_map_hero->register_element({ "4 down", "", [this] { game_hero_key(4, true); } });
	_game_map_hero->register_element({ "1 up", "", [this] { game_hero_key(1, false); } });
	_game_map_hero->register_element({ "2 up", "", [this] { game_hero_key(2, false); } });
	_game_map_hero->register_element({ "3 up", "", [this] { game_hero_key(3, false); } });
	_game_map_hero->register_element({ "4 up", "", [this] { game_hero_key(4, false); } });

	_settings = _output->create_menu();
	_settings->register_element({ "1", "zoom_in", [this] {settings_zoom_in(); } });
	_settings->register_element({ "2", "zoom_out", [this] {settings_zoom_out(); } });
	_settings->register_element({ "3 back to main menu button", "back", [this] {this->settings_back(); } });
	_settings->main(false);

	_output->show(_main_menu);

	_ready_message = _output->create_message();
	_ready_message->text("ready");
	_ready_message->time(xo::conf::MESSAGE_TIME);
	_ready_message->register_element({ "after", "", [this] {
		game_make_move(_next_x, _next_y);
	}});

	_player_one_message = _output->create_message();
	_player_one_message->text("player_1");
	_player_one_message->time(xo::conf::MESSAGE_TIME);
	_player_one_message->register_element({ "after", "", [this] {
		// _output->show(_ready_message);
		_output->show(_game_map_xo);
	}});

	_music_player = std::make_shared<MusicPlayer>();
	_music_player->load(xo::conf::FILE_MUSIC_INTRO);
	_music_player->play();
}

void xo::XOActionLogic::direct_execution()
{
	_output->run();
}

void XOActionLogic::main_menu_play()
{
	_output->show(_player_one_message);
	_music_player->load(xo::conf::FILE_MUSIC_GAME_XO);
	_music_player->play();
	//_output->show(_game_map_xo);
	// this->game_make_move(rand() % 3, rand() % 3);
	// this->_game_logic.print_status();
}

void XOActionLogic::main_menu_settings()
{
	_output->show(_settings);
	// this->_game_logic.restart_game(3, 3, PlayerSymbol::cross, 3);
	// this->_game_logic.print_status();
}

void XOActionLogic::main_menu_exit()
{
	_output->stop();
}

void XOActionLogic::settings_back()
{
	_output->show(_main_menu);
}

void XOActionLogic::settings_zoom_in()
{
	_scale *= 1.1f;
	_output->scale(_scale);
}

void XOActionLogic::settings_zoom_out()
{
	_scale /= 1.1f;
	_output->scale(_scale);
}

void XOActionLogic::game_hero_key(int key, bool down)
{
	_game_map_hero->button(key - 1, down);
}

void XOActionLogic::game_make_move(unsigned x, unsigned y)
{
	ChartReader(conf::FILE_MUSIC_GAME_HERO_TONES).put_notes_on(_game_map_hero);
	_output->show(_game_map_hero);
	_game_map_hero->start(0);
	_music_player->stop();
	_music_player->load(conf::FILE_MUSIC_INTRO);
	_music_player->play();

	/*PlayerSymbol next_symbol = _game_logic.player_to_play_next();
	bool result = _game_logic.play_move(x, y);
	if (result)
	{
		_game_map_xo->put(next_symbol, x, y);
	}

	if (_game_logic.current_state() == GameState::finished)
	{
		PlayerSymbol winner = _game_logic.winner();
		// zrób coœ z wygranym
		_output->show(_main_menu);
		_game_logic.restart_game(3, 3, PlayerSymbol::circle, 3);
	}
}*/

// legacy stuff
/*
void XOActionLogic::take_action(SelectionInfo selected_info)
{
	XOViewTag current_view = _view_manager.get_current_view();

	pressed pr = selected_info.first;
	elementNumber element_number = selected_info.second;

	unsigned number_of_fields = _game_logic.get_number_of_fields();


	if (current_view == XOViewTag::game)
	{
		if (pr == true) // jeœli wciœniêto przycisk
		{
			if (0 < element_number && element_number < number_of_fields)
			{ // to by³ przycik z planszy
				_game_logic.make_move(element_number, _game_logic.get_current_player());
				_game_logic.change_current_player();
				// rysownie
			}
		}
	}*/
}