#include "pch.h"
#include "XOActionLogic.h"
#include "ChartReader.h"
#include "XOTimer.h"


using namespace xo;

XOActionLogic::XOActionLogic(const std::shared_ptr<XOIOutput> &output,
	const std::shared_ptr<XOGameLogic> &xo_game_logic,
	const std::shared_ptr<XOHeroGameLogic> &hero_game_logic) :
	_xo_game_logic(xo_game_logic),
	_hero_game_logic(hero_game_logic),
	_scale(1.f),
	_random_distribution(conf::GAME_HERO_TURN_MUSIC_RANGE_BEGIN,
		conf::GAME_HERO_TURN_MUSIC_RANGE_END),
	_next_x(0),
	_next_y(0),
	_selected(false)
{
	_output = output;

	_hero_game_logic->on_note_omitted([this](auto note) { on_note_omitted(note); });

	_main_menu = _output->create_menu();
	_main_menu->register_element({ "1 play button", "play", [this] { this->main_menu_play(); } });
	_main_menu->register_element({ "2 settings button", "settings", [this] { this->main_menu_settings(); } });
	_main_menu->register_element({ "3 exit button", "exit", [this] { this->main_menu_exit(); } });

	_game_map_xo = _output->create_game_map_xo();
	_game_map_xo->width(3);
	_game_map_xo->height(3);
	for (int i = 0; i < 9; ++i)
	{
		auto col = i % 3;
		auto row = i / 3;
		std::string coords = std::to_string(col) + " " + std::to_string(row);
		_game_map_xo->register_element({ coords, "", [this, col, row]
		{
			on_xo_map_field_selected(col, row);
		} });
	}
	_game_map_xo->register_element({ "back", "", [this] {
		settings_back();
	} });

	_game_map_hero = _output->create_game_map_hero();
	_game_map_hero->register_element({ "1 down", "", [this] { game_hero_key(1, true); } });
	_game_map_hero->register_element({ "2 down", "", [this] { game_hero_key(2, true); } });
	_game_map_hero->register_element({ "3 down", "", [this] { game_hero_key(3, true); } });
	_game_map_hero->register_element({ "4 down", "", [this] { game_hero_key(4, true); } });
	_game_map_hero->register_element({ "1 up", "", [this] { game_hero_key(1, false); } });
	_game_map_hero->register_element({ "2 up", "", [this] { game_hero_key(2, false); } });
	_game_map_hero->register_element({ "3 up", "", [this] { game_hero_key(3, false); } });
	_game_map_hero->register_element({ "4 up", "", [this] { game_hero_key(4, false); } });
	_game_map_hero->life(.5f);
	_game_map_hero->progress(.75f);

	_settings = _output->create_menu();
	_settings->register_element({ "1", "zoom_in", [this] {settings_zoom_in(); } });
	_settings->register_element({ "2", "zoom_out", [this] {settings_zoom_out(); } });
	_settings->register_element({ "3", "map_size", [this] {settings_map_size(); } });
	_settings->register_element({ "4 back to main menu button", "back", [this] {this->settings_back(); } });
	_settings->main(false);

	_settings_size = _output->create_menu();
	_settings_size->register_element({ "1", "3x3", [this] { set_map_size(3, 3); main_menu_settings(); } });
	_settings_size->register_element({ "2", "4x4", [this] { set_map_size(4, 4); main_menu_settings(); } });
	_settings_size->register_element({ "3", "5x3", [this] { set_map_size(5, 3); main_menu_settings(); } });
	_settings_size->register_element({ "4", "back", [this] { main_menu_settings(); } });
	_settings_size->main(false);

	_output->show(_main_menu);

	_message_ready = _output->create_message();
	_message_ready->text("ready");
	_message_ready->time(xo::conf::MESSAGE_TIME);
	_message_ready->register_element({ "after", "", [this] {
		_output->show(_message_hero_steady);
	} });

	_message_hero_steady = _output->create_message();
	_message_hero_steady->text("steady");
	_message_hero_steady->time(xo::conf::MESSAGE_TIME);
	_message_hero_steady->register_element({ "after", "", [this] {
		_output->show(_message_hero_go);
	} });

	_message_hero_go = _output->create_message();
	_message_hero_go->text("go");
	_message_hero_go->time(xo::conf::MESSAGE_TIME);
	_message_hero_go->register_element({ "after", "", [this] {
		game_make_move(_next_x, _next_y);
	} });

	_message_player_o = _output->create_message();
	_message_player_o->text("player_o");
	_message_player_o->time(xo::conf::MESSAGE_TIME);
	_message_player_o->register_element({ "after", "", [this] {
		_resume_xo_map();
	} });

	_message_player_x = _output->create_message();
	_message_player_x->text("player_x");
	_message_player_x->time(xo::conf::MESSAGE_TIME);
	_message_player_x->register_element({ "after", "", [this] {
		_resume_xo_map();
	} });

	_message_hero_success = _output->create_message();
	_message_hero_success->text("success");
	_message_hero_success->time(xo::conf::MESSAGE_TIME);
	_message_hero_success->type(XOIMessage::Type::SUCCESS);
	_message_hero_success->register_element({ "after", "", [this] {
		main_menu_play();
	} });

	_message_hero_fail = _output->create_message();
	_message_hero_fail->text("fail");
	_message_hero_fail->time(xo::conf::MESSAGE_TIME);
	_message_hero_fail->type(XOIMessage::Type::FAIL);
	_message_hero_fail->register_element({ "after", "", [this] {
		main_menu_play();
	} });

	_message_hero_wins_player_x = _output->create_message();
	_message_hero_wins_player_x->text("player_x_wins");
	_message_hero_wins_player_x->time(xo::conf::MESSAGE_TIME);
	_message_hero_wins_player_x->type(XOIMessage::Type::SPECIAL);
	_message_hero_wins_player_x->register_element({ "after", "", [this] {
		settings_back();
	} });

	_message_hero_wins_player_o = _output->create_message();
	_message_hero_wins_player_o->text("player_o_wins");
	_message_hero_wins_player_o->time(xo::conf::MESSAGE_TIME);
	_message_hero_wins_player_o->type(XOIMessage::Type::SPECIAL);
	_message_hero_wins_player_o->register_element({ "after", "", [this] {
		settings_back();
	} });

	_message_hero_draw = _output->create_message();
	_message_hero_draw->text("draw");
	_message_hero_draw->time(xo::conf::MESSAGE_TIME);
	_message_hero_draw->register_element({ "after", "", [this] {
		settings_back();
	} });

	_music_player = std::make_shared<MusicPlayer>();
	_music_player->delegate_executor(_output);
	_music_player->load(xo::conf::FILE_MUSIC_INTRO);
	_music_player->play(0, -1, 0);

	_sound_effect_player = std::make_shared<MusicPlayer>();
	_sound_effect_player->delegate_executor(_output);
}

void xo::XOActionLogic::direct_execution()
{
	_output->run();
}

void XOActionLogic::main_menu_play()
{
	_xo_game_logic->restart_game(3,3, PlayerSymbol::circle, 3);
	_hero_game_logic->reset();
	_game_map_hero->reset();
	_game_map_xo->clear_selection();
	OutputDebugStringA("XOActionLogic:main_menu_play\n");
	if (_xo_game_logic->player_to_play_next() == PlayerSymbol::circle)
	{
		_output->show(_message_player_o);
	}
	else
	{
		_output->show(_message_player_x);
	}
	_music_player->stop();
	_music_player->load(xo::conf::FILE_MUSIC_GAME_XO);
	_music_player->play();
	//_output->show(_game_map_xo);
	// this->game_make_move(rand() % 3, rand() % 3);
	// this->_game_logic.print_status();
}

void XOActionLogic::main_menu_settings()
{
	_output->show(_settings);
	// this->_game_logic.restart_game(3, 3, PlayerSymbol::cross, 3);
	// this->_game_logic.print_status();
}

void XOActionLogic::main_menu_exit()
{
	_output->stop();
}

void XOActionLogic::settings_back()
{
	_music_player->stop();
	_music_player->load(conf::FILE_MUSIC_INTRO);
	_music_player->play();
	_output->show(_main_menu);
}

void XOActionLogic::settings_zoom_in()
{
	_scale *= 1.1f;
	_output->scale(_scale);
}

void XOActionLogic::settings_zoom_out()
{
	_scale /= 1.1f;
	_output->scale(_scale);
}

void xo::XOActionLogic::settings_map_size()
{
	_output->show(_settings_size);
}

void XOActionLogic::game_hero_key(int key, bool down)
{
	if (down)
	{
		auto note = _hero_game_logic->button_pressed(key - 1);
		_hero_game_logic->destroy_note(note);

		if (note)
		{
			OutputDebugStringA((std::stringstream() << note->time << '\n').str().c_str());
		}
		else
		{
			_sound_effect_player->load(conf::SOUND_EFFECT_CLICK_ARRAY[key - 1]);
			_sound_effect_player->play();
		}

		_game_map_hero->life(_hero_game_logic->progress());
	}
	_game_map_hero->button(key - 1, down);
}

// called after select
void XOActionLogic::game_make_move(unsigned x, unsigned y)
{
	auto notes = _hero_game_logic->load(conf::FILE_MUSIC_GAME_HERO_TONES);
	_game_map_hero->feed(notes);
	_output->show(_game_map_hero);

	_music_player->load(conf::FILE_MUSIC_INTRO);

	_game_map_hero->life(_hero_game_logic->progress());

	auto music_begin = 0;

	_music_player->play_event([this, music_begin]
	{
		_music_player->play_event(nullptr);
		_game_map_hero->start(0, music_begin + conf::GAME_HERO_PAUSE_BEFORE_FIRST_NOTE, conf::GAME_HERO_TURN_LENGTH);
		_hero_game_logic->start(0, music_begin + conf::GAME_HERO_PAUSE_BEFORE_FIRST_NOTE);
	});
	_music_player->end_event([this, x, y]
	{
		_on_hero_end(x, y);
	});
	_music_player->play(music_begin, music_begin + conf::GAME_HERO_TURN_LENGTH, conf::GAME_HERO_PAUSE_BEFORE_TURN);
}

// called on select
void XOActionLogic::on_xo_map_field_selected(int x, int y)
{
	if (!_selected && _xo_game_logic->peek_move(x, y))
	{
		_selected = true;
		_game_map_xo->select(_xo_game_logic->player_to_play_next(), x, y);
		XOTimer([this, x, y] {
			_next_x = x;
			_next_y = y;
			_output->show(_message_ready);
			_music_player->stop();
		}).after(conf::GAME_XO_SELECT_TIME);
	};
}

void xo::XOActionLogic::set_map_size(int x, int y)
{
	_xo_game_logic->restart_game(x, y, PlayerSymbol::circle, 3);
	_game_map_xo->width(x);
	_game_map_xo->height(y);
}

void XOActionLogic::_resume_xo_map()
{
	OutputDebugStringA("player_one_message:after\n");

	_selected = false;
	_game_map_hero->player_symbol(_xo_game_logic->player_to_play_next());
	_game_map_xo->player_symbol(_xo_game_logic->player_to_play_next());
	_output->show(_game_map_xo);
}

void xo::XOActionLogic::_on_hero_end(int x, int y)
{
	_music_player->end_event(nullptr);
	if (_hero_game_logic->won())
	{
		_game_map_xo->put(_xo_game_logic->player_to_play_next(), x, y);
		_xo_game_logic->play_move(x, y);

		if (_xo_game_logic->current_state() == GameState::finished)
		{
			auto winner = _xo_game_logic->winner();

			if (winner == PlayerSymbol::circle)
			{
				_output->show(_message_hero_wins_player_o);
			}
			else if (winner == PlayerSymbol::cross)
			{
				_output->show(_message_hero_wins_player_x);
			}
			else
			{
				_output->show(_message_hero_draw);
			}

			return;
		}

		_output->show(_message_hero_success);
	}
	else
	{
		_xo_game_logic->pass_turn();
		_output->show(_message_hero_fail);
	}
}

void XOActionLogic::on_note_omitted(const Note &note)
{
	_game_map_hero->life(_hero_game_logic->progress());
	OutputDebugStringA((std::stringstream() << "on_note_omitted " << note.tone << ' ' << note.time << '\n').str().c_str());
}
