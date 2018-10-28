#pragma once

#include "XOInput.h"
#include "XOViewManager.h"
#include "XOGameLogic.h"

namespace xo
{
	class XOActionLogic
	{
	public:
		XOActionLogic(XOViewManager &view_manager, XOGameLogic& game_logic);

		void direct_execution();
	private:
		XOViewManager &_view_manager;
		XOGameLogic &_game_logic;
	};
}
