#pragma once
#include "XOIOutput.h"

namespace xo
{
	class XOConsoleOutput : public XOIOutput
	{
	public:
		std::shared_ptr<IXOMenu> create_menu() override;
		std::shared_ptr<IXOGameMap> create_game_map() override;

		void show(const std::shared_ptr<IXOMenu> &) override;
		void show(const std::shared_ptr<IXOGameMap> &) override;

		void run() override;
	};
}
