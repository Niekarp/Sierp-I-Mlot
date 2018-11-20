// g³ówna klasa programu "Sierp I M³ot"

#pragma once

#include "XOIOutput.h"
#include "XOActionLogic.h"
#include "XOGameLogic.h"
#include "XOHeroGameLogic.h"

namespace xo {
	class XOApplication
	{
	public:
		static std::shared_ptr<XOApplication> get_instance();

		void run();
	private:
		XOApplication();
		static std::shared_ptr<XOApplication> _instance;

		std::shared_ptr<XOIOutput> _output;
		std::shared_ptr<XOActionLogic> _action_logic;
		std::shared_ptr<XOGameLogic> _xo_game_logic;
		std::shared_ptr<XOHeroGameLogic> _hero_game_logic;
	};
}