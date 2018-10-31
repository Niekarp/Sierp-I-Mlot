#pragma once
#include "XOIOutput.h"
#include "XOIMenu.h"
#include "XOIGameMap.h"
#include "Console.h"

namespace xo
{
	class XOConsoleOutput : public XOIOutput
	{
	public:
		XOConsoleOutput();

		std::shared_ptr<XOIMenu> create_menu() override;
		std::shared_ptr<XOIGameMap> create_game_map() override;

		void show(const std::shared_ptr<XOIMenu> &) override;
		void show(const std::shared_ptr<XOIGameMap> &) override;

		void run() override;
		void stop() override;
	private:
		std::shared_ptr<Console> _console;

		bool _everyone_stunned;
		void _amaze_them_with_the_intro(const std::shared_ptr<XOIMenu> &) override;
	};
}
