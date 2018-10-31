#pragma once
#include "XOIMenu.h"
#include "XOIGameMap.h"

namespace xo
{
	class XOIOutput
	{
	public:
		virtual std::shared_ptr<XOIMenu> create_menu() = 0;
		virtual std::shared_ptr<XOIGameMap> create_game_map() = 0;

		virtual void show(const std::shared_ptr<XOIMenu> &) = 0;
		virtual void show(const std::shared_ptr<XOIGameMap> &) = 0;

		virtual void run() = 0;
	};
}
