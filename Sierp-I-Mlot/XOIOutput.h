#pragma once
#include "XOIMenu.h"
#include "XOIGameMapXO.h"
#include "XOIGameMapHero.h"
#include "XOIMessage.h"

namespace xo
{
	class XOIOutput
	{
	public:
		virtual std::shared_ptr<XOIMenu> create_menu() = 0;
		virtual std::shared_ptr<XOIGameMapXO> create_game_map_xo() = 0;
		virtual std::shared_ptr<XOIGameMapHero> create_game_map_hero() = 0;
		virtual std::shared_ptr<XOIMessage> create_message() = 0;

		virtual void show(const std::shared_ptr<XOIMenu> &) = 0;
		virtual void show(const std::shared_ptr<XOIGameMapXO> &) = 0;
		virtual void show(const std::shared_ptr<XOIGameMapHero> &) = 0;
		virtual void show(const std::shared_ptr<XOIMessage> &) = 0;

		virtual void run() = 0;
		virtual void stop() = 0;

		virtual void scale(float) = 0;
	private:
		virtual void _amaze_them_with_the_intro(const std::shared_ptr<XOIMenu> &) = 0;
	};
}
