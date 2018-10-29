#include "pch.h"
#include "XOConsoleOutput.h"

#include "Console.h"
#include "SuperWaveAnimation.h"
#include "CenteredFramedPlane.h"
#include "FileImagePlane.h"
#include "ButtonPlane.h"
#include "FramePlaneFrameCreator.h"
#include "TextConsolePlane.h"
#include "Menu.h"
#include "MusicPlayer.h"
#include "IntroAnimation.h"
#include "AnimationChain.h"
#include "IntroMenuAnimation.h"
#include "ChartReader.h"

#include <chrono>

namespace xo
{
	XOConsoleOutput::XOConsoleOutput(XOActionLogic & action_logic) :
		XOIOutput(action_logic)
	{
	}

	void XOConsoleOutput::keep_drawing()
	{
		ChartReader reader("resources/notes.chart");
		auto notes = reader.notes();

		auto console = Console::get_instance();
		console->resolution(12);
		console->resize_window(800, 800);

		auto music_player = std::make_shared<MusicPlayer>();
		music_player->load("resources/sound.WAV");
		music_player->play();

		auto main_menu = std::make_shared<Menu>();
		int i = 12;
		main_menu->add_option({ "play", [&console, &i, this]
		{
			// console->resolution(i += 1);
			this->_action_logic.game_make_move(rand() % 3, rand() % 3);
			console->resize_window(800, 800);
		} });
		main_menu->add_option({ "settings", [&console, &i]
		{
			console->resolution(i -= 1);
			console->resize_window(800, 800);
		} });
		main_menu->add_option({ "exit", [&console]
		{
			console->stop();
		} });

		auto image1 = std::make_shared<FileImagePlane>("resources/animation_text1.txt");
		image1->color(FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		auto image2 = std::make_shared<FileImagePlane>("resources/animation_text2.txt");
		image2->color(FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		auto image3 = std::make_shared<FileImagePlane>("resources/animation_text3.txt");
		image3->color(FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		auto animated_image4 = std::make_shared<AnimatedFileImagePlane>();
		animated_image4->color(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		animated_image4->load_frame("resources/pblogo5.txt");
		animated_image4->load_frame("resources/pblogo8.txt");
		animated_image4->load_frame("resources/pblogo11.txt");
		animated_image4->load_frame("resources/pblogo14.txt");
		animated_image4->load_frame("resources/pblogo17.txt");
		animated_image4->load_frame("resources/pblogo20.txt");
		animated_image4->load_frame("resources/pblogo23.txt");

		auto intro_animation = std::make_shared<IntroAnimation>(image1, image2, image3, animated_image4);
		auto menu_animation = std::make_shared<SuperWaveAnimation>(8);
		auto intro_menu_animation = std::make_shared<IntroMenuAnimation>(IConsolePlane::Position{ 60, 40 });
		intro_menu_animation->fill((char)178, BACKGROUND_BLUE | BACKGROUND_INTENSITY);
		auto chain = std::make_shared<AnimationChain>();

		chain->add(intro_animation);
		chain->add(menu_animation);
		chain->add(intro_menu_animation);
		chain->overlap(100);
		chain->on_end([&main_menu, &console](auto animation_index)
		{
			if (animation_index == 2)
			{
				main_menu->draw_on(console);
			}
		});

		console->animate_async(chain, 80);

		console->exec();
	}
	void XOConsoleOutput::change_drawing_view(XOViewTag view)
	{

	}
	void XOConsoleOutput::draw_symbol(unsigned x, unsigned y)
	{

	}
}
