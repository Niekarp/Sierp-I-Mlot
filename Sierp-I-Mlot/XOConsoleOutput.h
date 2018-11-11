#pragma once
#include "XOIOutput.h"
#include "XOIMenu.h"
#include "XOIGameMapXO.h"
#include "Console.h"

namespace xo
{
	class XOConsoleOutput : public XOIOutput
	{
	public:
		XOConsoleOutput();

		std::shared_ptr<XOIMenu> create_menu() override;
		std::shared_ptr<XOIGameMapXO> create_game_map_xo() override;
		std::shared_ptr<XOIGameMapHero> create_game_map_hero() override;

		void show(const std::shared_ptr<XOIMenu> &) override;
		void show(const std::shared_ptr<XOIGameMapXO> &) override;
		void show(const std::shared_ptr<XOIGameMapHero> &) override;

		void run() override;
		void stop() override;

		void scale(float) override;
	private:
		std::shared_ptr<Console> _console;
		float _scale;

		bool _everyone_stunned;
		void _amaze_them_with_the_intro(const std::shared_ptr<XOIMenu> &) override;
	};
}
