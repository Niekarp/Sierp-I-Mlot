#include "pch.h"
#include "XOConsoleOutput.h"

#include "console/Console.h"
#include "console/SuperWaveAnimation.h"
#include "console/CenteredFramedPlane.h"
#include "console/FileImagePlane.h"
#include "console/ButtonPlane.h"
#include "console/FramePlaneFrameCreator.h"
#include "console/TextConsolePlane.h"
#include "console/Menu.h"
#include "console/MusicPlayer.h"
#include "console/IntroAnimation.h"
#include "console/AnimationChain.h"
#include "console/IntroMenuAnimation.h"
#include "console/ChartReader.h"

#include <chrono>

namespace xo
{
	XOConsoleOutput::XOConsoleOutput(XOActionLogic & action_logic) :
		XOIOutput(action_logic)
	{
	}

	void XOConsoleOutput::keep_drawing()
	{
		ChartReader reader("notes.chart");
		auto notes = reader.notes();

		auto console = Console::get_instance();
		console->resolution(12);
		console->resize_window(800, 800);


		//
		/*auto buffer = console->create_buffer();
		auto start = std::chrono::system_clock::now();

		console->do_every(5, [&console, &buffer, &notes, &start]
		{
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::system_clock::now() - start);

			/*if (n.timestamp / 40 % 100 == 0)
			{
				buffer->clear(' ', 0);
			}

			for (auto &n : notes)
			{
				if (n.timestamp > duration.count())
				{
					break;
				}

				buffer->put(n.timestamp / 40 % 100, n.key * 4 + 4, ' ', BACKGROUND_RED);
			}


			console->draw(buffer);
		});

		console->exec();*/
		//

		auto music_player = std::make_shared<MusicPlayer>();
		music_player->load("sound.WAV");
		music_player->play();

		auto main_menu = std::make_shared<Menu>();
		int i = 12;
		main_menu->add_option({ "play", [&console, &i]
		{
			console->resolution(i += 1);
			console->resize_window(800, 800);
		} });
		main_menu->add_option({ "settings", [&console, &i]
		{
			console->resolution(i -= 1);
			console->resize_window(800, 800);
		} });
		main_menu->add_option({ "exit", []
		{
			ExitProcess(0);
		} });

		auto image1 = std::make_shared<FileImagePlane>("animation_text1.txt");
		image1->color(FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		auto image2 = std::make_shared<FileImagePlane>("animation_text2.txt");
		image2->color(FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		auto image3 = std::make_shared<FileImagePlane>("animation_text3.txt");
		image3->color(FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		auto animated_image4 = std::make_shared<AnimatedFileImagePlane>();
		animated_image4->color(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		animated_image4->load_frame("pblogo5.txt");
		animated_image4->load_frame("pblogo8.txt");
		animated_image4->load_frame("pblogo11.txt");
		animated_image4->load_frame("pblogo14.txt");
		animated_image4->load_frame("pblogo17.txt");
		animated_image4->load_frame("pblogo20.txt");
		animated_image4->load_frame("pblogo23.txt");

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
