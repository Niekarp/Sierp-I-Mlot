// g³ówna klasa programu "Sierp I M³ot"

#pragma once

#include "XOViewManager.h"
#include "XOActionLogic.h"
#include "XOGameLogic.h"
#include "XOClient.h"

namespace xo {
	class XOApplication
	{
	public:
		static XOApplication *get_instance();

		void run();
	private:
		XOApplication();
		static XOApplication *_instance;

		XOViewManager _view_manager;
		XOActionLogic _action_logic;
		XOGameLogic _game_logic;
		XOClient _client;
	};
}