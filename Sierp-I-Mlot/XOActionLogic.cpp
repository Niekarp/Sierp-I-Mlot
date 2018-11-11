#include "pch.h"
#include "XOActionLogic.h"

namespace xo
{
	XOActionLogic::XOActionLogic(std::shared_ptr<XOIOutput> &output, XOGameLogic &game_logic) :
		_game_logic(game_logic),
		_scale(1.f)
	{
		_output = output;

		_main_menu = _output->create_menu();
		_main_menu->register_element({ "play button", "play", [this] { this->main_menu_play(); } });
		_main_menu->register_element({ "settings button", "settings", [this] { this->main_menu_settings(); } });
		_main_menu->register_element({ "exit button", "exit", [this] { this->main_menu_exit(); } });

		_game_map = _output->create_game_map();
		_game_map->width(3);
		_game_map->height(3);
		for (int i = 0; i < 9; ++i)
		{
			auto col = i % 3;
			auto row = i / 3;
			std::string coords = std::to_string(col) + " " + std::to_string(row);
			_game_map->register_element({ coords, "", [this, col, row]
			{
				this->game_make_move(col, row);
			} });
		}

		_settings = _output->create_menu();
		_settings->register_element({ "back to main menu button", "back", [this] {this->settings_back(); } });
		_settings->register_element({ "", "zoom in", [this] {settings_zoom_in(); } });
		_settings->register_element({ "", "zoom out", [this] {settings_zoom_out(); } });
		_settings->main(false);

		_output->show(_main_menu);
	}

	void xo::XOActionLogic::direct_execution()
	{
		_output->run();
	}

	void XOActionLogic::main_menu_play()
	{
		_output->show(_game_map);
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
		_scale *= 1.25f;
		_output->scale(_scale);
	}

	void XOActionLogic::settings_zoom_out()
	{
		_scale *= 0.75;
		_output->scale(_scale);
	}

	void XOActionLogic::game_make_move(unsigned x, unsigned y)
	{
		PlayerSymbol next_symbol = _game_logic.player_to_play_next();
		bool result = _game_logic.play_move(x, y);
		if (result)
		{
			_game_map->put(next_symbol, x, y);
		}

		if (_game_logic.current_state() == GameState::finished)
		{
			PlayerSymbol winner = _game_logic.winner();
			// zrób coœ z wygranym
			_output->show(_main_menu);
			_game_logic.restart_game(3, 3, PlayerSymbol::circle, 3);
		}
	}

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
		}
	}
	*/
}