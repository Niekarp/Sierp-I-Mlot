#include "pch.h"
#include "XOActionLogic.h"
/*
namespace xo
{
	XOActionLogic::XOActionLogic(std::shared_ptr<XOIOutput> output, XOGameLogic& game_logic) :
		_output(output),
		_game_logic(game_logic)
	{
	}

	void xo::XOActionLogic::direct_execution()
	{
		//_output->keep_drawing();
	}


	void XOActionLogic::main_menu_play()
	{
		_output->change_drawing_view(XOViewTag::game);
	}

	void XOActionLogic::main_menu_settings()
	{
		_output->change_drawing_view(XOViewTag::settings);
	}

	void XOActionLogic::main_menu_exit()
	{
		// wychodzenie przypisane bezpoœrednio do przycisku
	}

	void XOActionLogic::game_make_move(unsigned x, unsigned y)
	{
		if (_game_logic.play_move(x, y))
		{
			_output->draw_symbol(x, y);
		}
		if (_game_logic.current_state() == GameState::finished)
		{
			PlayerSymbol winner = _game_logic.winner();
			// zrób coœ z wygranym
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
	
}*/