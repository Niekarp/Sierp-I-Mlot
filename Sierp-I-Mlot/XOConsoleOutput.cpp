#include "pch.h"
#include "XOConsoleOutput.h"
#include "XOConsoleMenu.h"
#include "XOConsoleGameMapXO.h"

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
#include "XOConsoleGameMapHero.h"
#include "XOConsoleMessage.h"

#include <chrono>

namespace xo
{
	XOConsoleOutput::XOConsoleOutput() :
		_scale(1),
		_background_animation(true),
		_transition_from_menu(true)
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
	
	std::shared_ptr<XOIGameMapXO> XOConsoleOutput::create_game_map_xo()
	{
		auto map = std::make_shared<XOConsoleGameMapXO>();
		map->field_o(conf::FILE_O_FIELD);
		map->field_x(conf::FILE_X_FIELD);
		return map;
	}

	std::shared_ptr<XOIGameMapHero> XOConsoleOutput::create_game_map_hero()
	{
		return std::shared_ptr<XOIGameMapHero>(new XOConsoleGameMapHero());
	}

	std::shared_ptr<XOIMessage> XOConsoleOutput::create_message()
	{
		return std::shared_ptr<XOIMessage>(new XOConsoleMessage());
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
		else if (!_transition_from_menu)
		{
			auto chain = std::make_shared<AnimationChain>();
			_open_menu_animation(menu, chain, true);
			_console->animate_async(chain, conf::ANIMATION_FRAME_FREQUENCY);
		}
		auto console_menu = std::dynamic_pointer_cast<XOConsoleMenu>(menu);
		console_menu->clear(_console);
		console_menu->draw_on(_console);
		_transition_from_menu = true;
	}
	
	void XOConsoleOutput::show(const std::shared_ptr<XOIGameMapXO> &game_map)
	{
		_run_background_animation();
		auto console_game_map = std::dynamic_pointer_cast<XOConsoleGameMapXO>(game_map);
		console_game_map->draw_on(_console);
		_transition_from_menu = false;
	}

	void XOConsoleOutput::show(const std::shared_ptr<XOIGameMapHero> &game_map)
	{
		_run_background_animation();
		auto console_game_map = std::dynamic_pointer_cast<XOConsoleGameMapHero>(game_map);
		console_game_map->draw_on(_console);
		_background_animation = false;
		_transition_from_menu = false;
	}

	void XOConsoleOutput::show(const std::shared_ptr<XOIMessage> &message)
	{
		_background_animation = false;
		auto console_message = std::dynamic_pointer_cast<XOConsoleMessage>(message);
		console_message->draw_on(_console);
		_transition_from_menu = false;
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
		
		auto chain = std::make_shared<AnimationChain>();

		chain->add(intro_animation);
		_open_menu_animation(menu_after_intro, chain, false);
		chain->overlap(100);

		_console->animate_async(chain, xo::conf::ANIMATION_FRAME_FREQUENCY);
		_console->key_down_event([this](auto key)
		{
			if (key == VK_SPACE)
			{
				_console->frame(100000);
			}
		});

		_background_animation = true;

		// _console->exec();
	}

	void XOConsoleOutput::_open_menu_animation(const std::shared_ptr<XOIMenu> &menu,
		const std::shared_ptr<AnimationChain> &chain, bool reopen)
	{
		std::shared_ptr<SuperWaveAnimation> menu_animation;
		if (reopen)
		{
			menu_animation = std::make_shared<SuperWaveAnimation>(400, 8);
		}
		else
		{
			menu_animation = std::make_shared<SuperWaveAnimation>(1350, 8);
		}
		menu_animation->speed(3);
		auto intro_menu_animation = std::make_shared<IntroMenuAnimation>(IConsolePlane::Position{ 60, 40 });
		intro_menu_animation->fill((char)178, BACKGROUND_BLUE | BACKGROUND_INTENSITY);

		chain->add(menu_animation);
		chain->add(intro_menu_animation);
		chain->on_end([&menu, this](auto animation_index)
		{
			if (animation_index == 2)
			{
				this->show(menu);
				// std::static_pointer_cast<XOConsoleMenu>(menu_after_intro)->draw_on(_console);
			}
		});
	}
	void XOConsoleOutput::_run_background_animation()
	{
		if (_background_animation)
		{
			return;
		}

		//_console->clear();

		_background_animation = true;
		//auto menu_animation = std::make_shared<SuperWaveAnimation>(600, 8);
		auto chain_animation = std::make_shared<AnimationChain>();
		//chain_animation->add(menu_animation);
		_console->animate_async(chain_animation, xo::conf::ANIMATION_FRAME_FREQUENCY);
	}
	void XOConsoleOutput::execute(const std::function<void()> &delegate)
	{
		_console->execute(delegate);
	}
}
