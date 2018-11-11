#pragma once

#include "XOIOutput.h"
#include "XOGameLogic.h"

namespace xo
{
	class XOIOutput;

	class XOActionLogic
	{
	public:
		XOActionLogic(std::shared_ptr<XOIOutput> &, XOGameLogic &);

		void direct_execution();

		void main_menu_play();
		void main_menu_settings();
		void main_menu_exit();

		void settings_back();
		void settings_zoom_in();
		void settings_zoom_out();

		void game_make_move(unsigned x, unsigned y);

	private:
		std::shared_ptr<XOIOutput> _output;
		XOGameLogic &_game_logic;
		float _scale;

		std::shared_ptr<XOIMenu> _main_menu;
		std::shared_ptr<XOIGameMap> _game_map;
		std::shared_ptr<XOIMenu> _settings;
	};
}
