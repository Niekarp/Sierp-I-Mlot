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
		static std::shared_ptr<XOApplication> get_instance();

		void run();
	private:
		XOApplication();
		static std::shared_ptr<XOApplication> _instance;

		XOViewManager _view_manager;
		XOActionLogic _action_logic;
		XOGameLogic _game_logic;
		XOClient _client;
	};
}