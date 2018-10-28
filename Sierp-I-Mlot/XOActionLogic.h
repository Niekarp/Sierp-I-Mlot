#pragma once

#include "XOInput.h"
#include "XOIOutput.h"
#include "XOGameLogic.h"

namespace xo
{
	class XOActionLogic
	{
	public:
		XOActionLogic(XOIOutput &output, XOGameLogic& game_logic);

		void direct_execution();
	private:
		XOIOutput &_output;
		XOGameLogic &_game_logic;
	};
}
