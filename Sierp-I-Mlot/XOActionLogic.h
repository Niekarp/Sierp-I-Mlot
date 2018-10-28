#pragma once

#include "XOIOutput.h"
#include "XOGameLogic.h"

namespace xo
{
	class XOIOutput;

	class XOActionLogic
	{
	public:
		XOActionLogic(
			std::shared_ptr<XOIOutput> output,
			XOGameLogic& game_logic);

		void direct_execution();

		void main_menu_play();
		void main_menu_settings();
		void main_menu_exit();

		void game_make_move(unsigned x, unsigned y);
	private:
		std::shared_ptr<XOIOutput> _output;
		XOGameLogic &_game_logic;
	};
}
