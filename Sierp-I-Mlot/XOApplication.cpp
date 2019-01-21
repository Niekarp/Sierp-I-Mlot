#include "pch.h"
#include "XOApplication.h"
#include "XOInput.h"
#include "XOConsoleOutput.h"

namespace xo
{
	XOApplication::XOApplication()
	{
	}

	void XOApplication::initialize()
	{
		_xo_game_logic = std::make_shared<XOGameLogic>(3, 3, PlayerSymbol::circle, 3);
		_hero_game_logic = std::make_shared<XOHeroGameLogic>();		
		_output = create_output();
		_action_logic = std::make_shared<XOActionLogic>(_output, _xo_game_logic, _hero_game_logic);
	}

	void XOApplication::run()
	{
		_action_logic->direct_execution();
	}
}