#include "pch.h"
#include "XOActionLogic.h"

namespace xo
{
	XOActionLogic::XOActionLogic(std::shared_ptr<XOIOutput> &output, XOGameLogic &game_logic) :
		_game_logic(game_logic)
	{
		_output = output;

		_main_menu = _output->create_menu();
		_main_menu->register_element({ "play button", "PLAY", [this] { this->main_menu_play(); } });
		_main_menu->register_element({ "settings button", "SETTINGS", [this] { this->main_menu_settings(); } });
		_main_menu->register_element({ "exit button", "EXIT", [this] { this->main_menu_exit(); } });

		_output->show(_main_menu);
	}

	void xo::XOActionLogic::direct_execution()
	{
		_output->run();
	}

	void XOActionLogic::main_menu_play()
	{
		_output->show(_game_map);
	}

	void XOActionLogic::main_menu_settings()
	{
		_output->show(_settings);
	}

	void XOActionLogic::main_menu_exit()
	{
		_output->stop();
	}

	void XOActionLogic::game_make_move(unsigned x, unsigned y)
	{
		//if (_game_logic.play_move(x, y))
		//{
		//	// _output->draw_symbol(x, y);
		//}
		//if (_game_logic.current_state() == GameState::finished)
		//{
		//	PlayerSymbol winner = _game_logic.winner();
		//	// zrób coœ z wygranym
		//}
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