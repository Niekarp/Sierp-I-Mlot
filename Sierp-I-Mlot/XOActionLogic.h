#pragma once

#include "XOInput.h"
#include "XOViewManager.h"
#include "XOGameLogic.h"

namespace xo
{
	class XOActionLogic
	{
	public:
		XOActionLogic(const XOViewManager& view_manager, const XOGameLogic& game_logic);
		~XOActionLogic();

		void take_action(SelectionInfo selection_info);
		bool quit_pressed();
	private:
		XOViewManager& _view_manager;
		XOGameLogic& _game_logic;
	};
}
