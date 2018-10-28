#pragma once

#include "XOIOutput.h"
#include "XOGameLogic.h"

namespace xo
{
	class XOIOutput;

	class XOActionLogic
	{
	public:
		XOActionLogic(std::shared_ptr<XOIOutput> output, XOGameLogic& game_logic);

		void direct_execution();
	private:
		std::shared_ptr<XOIOutput> _output;
		XOGameLogic &_game_logic;
	};
}
