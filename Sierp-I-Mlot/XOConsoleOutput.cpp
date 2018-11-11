#include "pch.h"
#include "XOConsoleOutput.h"
#include "XOConsoleMenu.h"
#include "XOConsoleGameMap.h"

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
	XOConsoleOutput::XOConsoleOutput() :
		_scale(1)
	{
		_everyone_stunned = false;

		_console = Console::get_instance();
		_console->resolution(_scale * 12);
		_console->resize_window(900, 900);
	}

	std::shared_ptr<XOIMenu> XOConsoleOutput::create_menu()
	{
		return std::shared_ptr<XOIMenu>(new XOConsoleMenu());
	}
	
	std::shared_ptr<XOIGameMap> XOConsoleOutput::create_game_map()
	{
		return std::shared_ptr<XOIGameMap>(new XOConsoleGameMap());
	}
	
	void XOConsoleOutput::show(const std::shared_ptr<XOIMenu>& menu)
	{
		if (_everyone_stunned == false)
		{
			const std::shared_ptr<XOIMenu> &then_menu = menu;

			_amaze_them_with_the_intro(then_menu);
			_everyone_stunned = true;
			return;
		}
		auto console_menu = std::dynamic_pointer_cast<XOConsoleMenu>(menu);
		console_menu->clear(_console);
		console_menu->draw_on(_console);
	}
	
	void XOConsoleOutput::show(const std::shared_ptr<XOIGameMap> &game_map)
	{
		auto console_game_map = std::dynamic_pointer_cast<XOConsoleGameMap>(game_map);
		//console_game_map->clear(_console);
		console_game_map->draw_on(_console);
	}
	
	void XOConsoleOutput::run()
	{
		//// ChartReader reader("resources/notes.chart");
		//// auto notes = reader.notes();

		//auto music_player = std::make_shared<MusicPlayer>();
		//music_player->load("resources/sound.WAV");
		//music_player->play();

		////auto main_menu = std::make_shared<Menu>();
		////int i = 12;
		////main_menu->add_option({ "play", [&i, this]
		////{
		////	// this->_console->resize_window(800, 800);
		////} });
		////main_menu->add_option({ "settings", [this, &i]
		////{
		////	// _console->resolution(i -= 1);
		////	// _console->resize_window(800, 800);
		////} });
		////main_menu->add_option({ "exit", [this]
		////{
		////	
		////} });

		//auto image1 = std::make_shared<FileImagePlane>("resources/animation_text1.txt");
		//image1->color(FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		//auto image2 = std::make_shared<FileImagePlane>("resources/animation_text2.txt");
		//image2->color(FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		//auto image3 = std::make_shared<FileImagePlane>("resources/animation_text3.txt");
		//image3->color(FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		//auto animated_image4 = std::make_shared<AnimatedFileImagePlane>();
		//animated_image4->color(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		//animated_image4->load_frame("resources/pblogo5.txt");
		//animated_image4->load_frame("resources/pblogo8.txt");
		//animated_image4->load_frame("resources/pblogo11.txt");
		//animated_image4->load_frame("resources/pblogo14.txt");
		//animated_image4->load_frame("resources/pblogo17.txt");
		//animated_image4->load_frame("resources/pblogo20.txt");
		//animated_image4->load_frame("resources/pblogo23.txt");

		//auto intro_animation = std::make_shared<IntroAnimation>(image1, image2, image3, animated_image4);
		//auto menu_animation = std::make_shared<SuperWaveAnimation>(8);
		//auto intro_menu_animation = std::make_shared<IntroMenuAnimation>(IConsolePlane::Position{ 60, 40 });
		//intro_menu_animation->fill((char)178, BACKGROUND_BLUE | BACKGROUND_INTENSITY);
		//auto chain = std::make_shared<AnimationChain>();

		//chain->add(intro_animation);
		//chain->add(menu_animation);
		//chain->add(intro_menu_animation);
		//chain->overlap(100);
		//chain->on_end([&main_menu, this](auto animation_index)
		//{
		//	if (animation_index == 2)
		//	{
		//		main_menu->draw_on(_console);
		//	}
		//});

		//_console->animate_async(chain, 80);

		_console->exec();
	}

	void XOConsoleOutput::stop()
	{
		_console->stop();
	}

	void XOConsoleOutput::scale(float scale)
	{
		_scale = scale;
		_console->resolution(_scale * 12);
	}

	void XOConsoleOutput::_amaze_them_with_the_intro(const std::shared_ptr<XOIMenu> &menu_after_intro)
	{
		auto music_player = std::make_shared<MusicPlayer>();
		music_player->load("resources/sound.WAV");
		music_player->play();

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
		chain->on_end([&menu_after_intro, this](auto animation_index)
		{
			if (animation_index == 2)
			{
				this->show(menu_after_intro);
				// std::static_pointer_cast<XOConsoleMenu>(menu_after_intro)->draw_on(_console);
			}
		});

		_console->animate_async(chain, 80);
		_console->key_down_event([this](auto key)
		{
			if (key == VK_SPACE)
			{
				_console->frame(100000);
			}
		});

		// _console->exec();
	}
}
