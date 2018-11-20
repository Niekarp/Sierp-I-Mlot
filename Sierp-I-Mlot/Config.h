#pragma once

namespace xo
{
	namespace conf
	{
		const int ANIMATION_FRAME_FREQUENCY = 50;
		const size_t MESSAGE_TIME = 1000;

		const char *const FONT_SMALL_DIRECTORY = "resources/letters_small";
		const char *const FONT_NORMAL_DIRECTORY = "resources/letters_big";

		const int FONT_SMALL_SIZE_X = 3;
		const int FONT_SMALL_SIZE_Y = 4;

		const char *const FILE_MUSIC_INTRO = "resources/sound.WAV";
		const char *const FILE_MUSIC_GAME_XO = "resources/sound2.WAV";

		const int GAME_MAP_XO_SIZE_X = 60;
		const int GAME_MAP_XO_SIZE_Y = 30;

		const char *const FILE_X_FIELD = "resources/x_field.txt";
		const char *const FILE_O_FIELD = "resources/o_field.txt";

		const char *const FILE_MUSIC_GAME_HERO= "resources/songs/1/sound.WAV";
		const char *const FILE_MUSIC_GAME_HERO_TONES = "resources/songs/1/tones.txt";
		const char *const FILE_NOTE = "resources/note.txt";

		const int GAME_HERO_LIFE_BAR_SIZE_X = 15;
		const int GAME_HERO_LIFE_BAR_SIZE_Y = 30;
		const int GAME_HERO_LIFE_BAR_OFFSET_X = 5;

		const int GAME_HERO_PROGRESS_BAR_SIZE_X = 80;
		const int GAME_HERO_PROGRESS_BAR_SIZE_Y = 3;
		const int GAME_HERO_PROGRESS_BAR_OFFSET_Y = 3;

		const int GAME_HERO_LOGIC_NOTE_TOLERANCE = 400;
		const int GAME_HERO_PROGRESS_MAX = 100;
		const int GAME_HERO_PROGRESS_PENALTY_DROP = -6;
		const int GAME_HERO_PROGRESS_PENALTY_WRONG_CLICK = -20;
		const int GAME_HERO_PROGRESS_REWARD_GOOD_CLICK = 3;
		const int GAME_HERO_TURN_LENGTH = 30000;
		const int GAME_HERO_TURN_MUSIC_RANGE_BEGIN = 0;
		const int GAME_HERO_TURN_MUSIC_RANGE_END = 100000;
		const int GAME_HERO_PAUSE_BEFORE_TURN = 2000;
		const int GAME_HERO_PAUSE_BEFORE_FIRST_NOTE = 2000;

		const char *const SOUND_EFFECT_CLICK_ARRAY[] = {
			"resources/sounds/sound1.wav",
			"resources/sounds/sound2.wav",
			"resources/sounds/sound3.wav",
			"resources/sounds/sound4.wav"
		};

		const int GAME_XO_SELECT_FLASHING_SPEED = 10;
		const int GAME_XO_SELECT_TIME = 2000;
		const char * const GAME_XO_BACK_IMAGE = "resources/xo_menu_back.txt";
	}
}