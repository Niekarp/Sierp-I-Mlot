#include "pch.h"
#include "XOApplication.h"
#include "XOInput.h"
#include "XOConsoleOutput.h"

namespace xo
{
	std::shared_ptr<XOApplication> XOApplication::_instance = nullptr;
	std::shared_ptr<XOApplication> XOApplication::get_instance()
	{
		if (_instance == nullptr) _instance = std::shared_ptr<XOApplication>(new XOApplication());
		return _instance;
	}

	XOApplication::XOApplication() :
		_output(new XOConsoleOutput()),
		_xo_game_logic(std::make_shared<XOGameLogic>(3, 3, PlayerSymbol::circle, 3)),
		_hero_game_logic(std::make_shared<XOHeroGameLogic>())
	{
		_action_logic = std::make_shared<XOActionLogic>(_output, _xo_game_logic, _hero_game_logic);
	}

	void XOApplication::run()
	{
		_action_logic->direct_execution();
	}
}