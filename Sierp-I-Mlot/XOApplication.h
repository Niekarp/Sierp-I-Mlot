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
		XOApplication();
		void initialize();
		void run();

	protected:
		virtual std::shared_ptr<XOIOutput> create_output() = 0;

	private:
		std::shared_ptr<XOIOutput> _output;
		std::shared_ptr<XOActionLogic> _action_logic;
		std::shared_ptr<XOGameLogic> _xo_game_logic;
		std::shared_ptr<XOHeroGameLogic> _hero_game_logic;
	};
}