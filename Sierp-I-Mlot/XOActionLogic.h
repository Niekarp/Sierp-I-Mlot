#pragma once

#include "XOIOutput.h"
#include "XOGameLogic.h"
#include "MusicPlayer.h"
#include "XOIMessage.h"
#include "XOHeroGameLogic.h"

namespace xo
{
	class XOIOutput;

	class XOActionLogic
	{
	public:
		XOActionLogic(const std::shared_ptr<XOIOutput> &,
			const std::shared_ptr<XOGameLogic> &,
			const std::shared_ptr<XOHeroGameLogic> &);

		void direct_execution();

		void main_menu_play();
		void main_menu_settings();
		void main_menu_exit();

		void settings_back();
		void settings_zoom_in();
		void settings_zoom_out();
		void settings_map_size();
		void game_hero_key(int key, bool down);

		void game_make_move(unsigned x, unsigned y);
		void on_note_omitted(const Note &);
		void on_xo_map_field_selected(int x, int y);
		void set_map_size(int x, int y);

	private:
		void _resume_xo_map();
		void _on_hero_end(int x, int y);

		float _scale;
		unsigned _next_x;
		unsigned _next_y;
		bool _selected;

		std::shared_ptr<XOIOutput> _output;
		std::shared_ptr<XOGameLogic> _xo_game_logic;
		std::shared_ptr<XOHeroGameLogic> _hero_game_logic;
		std::shared_ptr<MusicPlayer> _music_player;
		std::shared_ptr<MusicPlayer> _sound_effect_player;
		std::shared_ptr<XOIMenu> _main_menu;
		std::shared_ptr<XOIGameMapXO> _game_map_xo;
		std::shared_ptr<XOIGameMapHero> _game_map_hero;
		std::shared_ptr<XOIMenu> _settings;
		std::shared_ptr<XOIMenu> _settings_size;
		std::shared_ptr<XOIMessage> _message_player_x;
		std::shared_ptr<XOIMessage> _message_player_o;
		std::shared_ptr<XOIMessage> _message_ready;
		std::shared_ptr<XOIMessage> _message_hero_steady;
		std::shared_ptr<XOIMessage> _message_hero_go;
		std::shared_ptr<XOIMessage> _message_hero_success;
		std::shared_ptr<XOIMessage> _message_hero_fail;
		std::shared_ptr<XOIMessage> _message_hero_wins_player_x;
		std::shared_ptr<XOIMessage> _message_hero_wins_player_o;
		std::shared_ptr<XOIMessage> _message_hero_draw;

		std::default_random_engine _random_engine;
		std::uniform_int_distribution<> _random_distribution;
	};
}
