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
	private:
		std::shared_ptr<Console> _console;
	};
}
