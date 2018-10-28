// g³ówna klasa programu "Sierp I M³ot"

#pragma once

#include "XOIOutput.h"
#include "XOActionLogic.h"
#include "XOGameLogic.h"

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
		XOActionLogic _action_logic;
		XOGameLogic _game_logic;
	};
}