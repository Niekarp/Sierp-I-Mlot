#pragma once
#include "IXOMenu.h"
#include "IXOGameMap.h"

namespace xo
{
	class XOIOutput
	{
	public:
		virtual std::shared_ptr<IXOMenu> create_menu() = 0;
		virtual std::shared_ptr<IXOGameMap> create_game_map() = 0;

		virtual void show(const std::shared_ptr<IXOMenu> &) = 0;
		virtual void show(const std::shared_ptr<IXOGameMap> &) = 0;

		virtual void run() = 0;
	};
}
