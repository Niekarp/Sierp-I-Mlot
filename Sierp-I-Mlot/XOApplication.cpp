#include "pch.h"
#include "XOApplication.h"
#include "XOInput.h"

namespace xo
{
	std::shared_ptr<XOApplication> XOApplication::_instance = nullptr;
	std::shared_ptr<XOApplication> XOApplication::get_instance()
	{
		if (_instance == nullptr) _instance = std::make_shared<XOApplication>(new XOApplication());
		return _instance;
	}

	XOApplication::XOApplication() :
		_action_logic(_view_manager, _game_logic)
	{
		_client.startxd();
	}

	void XOApplication::run()
	{
		while (true)
		{
			SelectionInfo element_selection =
				XOInput::get_element_selection(_view_manager.get_current_view());

			_action_logic.take_action(element_selection);

			if (_action_logic.quit_pressed()) break;
		}
	}
}