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
		_output(*(new XOConsoleOutput(_action_logic))),
		_action_logic(_output, _game_logic),
		_game_logic(3, 3, PlayerSymbol::circle, 3)
	{
	}

	void XOApplication::run()
	{
		_action_logic.direct_execution();
	}
}