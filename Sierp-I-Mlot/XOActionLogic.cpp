#include "pch.h"
#include "XOActionLogic.h"

namespace xo
{
	XOActionLogic::XOActionLogic(XOViewManager& view_manager, XOGameLogic& game_logic) :
		_view_manager(view_manager),
		_game_logic(game_logic)
	{
	}

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
}