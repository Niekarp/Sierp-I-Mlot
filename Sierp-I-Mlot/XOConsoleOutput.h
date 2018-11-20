#pragma once
#include "XOIOutput.h"
#include "XOIMenu.h"
#include "XOIGameMapXO.h"
#include "Console.h"
#include "AnimationChain.h"

namespace xo
{
	class XOConsoleOutput : public XOIOutput
	{
	public:
		XOConsoleOutput();

		std::shared_ptr<XOIMenu> create_menu() override;
		std::shared_ptr<XOIGameMapXO> create_game_map_xo() override;
		std::shared_ptr<XOIGameMapHero> create_game_map_hero() override;
		std::shared_ptr<XOIMessage> create_message() override;

		void show(const std::shared_ptr<XOIMenu> &) override;
		void show(const std::shared_ptr<XOIGameMapXO> &) override;
		void show(const std::shared_ptr<XOIGameMapHero> &) override;
		void show(const std::shared_ptr<XOIMessage> &) override;

		void run() override;
		void stop() override;

		void scale(float) override;

		void execute(const std::function<void()> &) override;

	private:
		void _amaze_them_with_the_intro(const std::shared_ptr<XOIMenu> &);
		void _open_menu_animation(const std::shared_ptr<XOIMenu> &, const std::shared_ptr<AnimationChain> &, bool reopen);
		void _run_background_animation();

		std::shared_ptr<Console> _console;
		float _scale;
		bool _background_animation;
		bool _everyone_stunned;
		bool _transition_from_menu;
	};
}
