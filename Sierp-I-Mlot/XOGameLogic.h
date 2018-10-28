#pragma once

#include <utility>

namespace xo
{
	enum class PlayerSymbol
	{
		cross,
		circle,
		none
	};
	typedef PlayerSymbol SquareState;

	enum class GameState
	{
		ongoing,
		stopped,
		finished
	};


	class XOGameLogic
	{
		struct BoardInfo
		{
			// sizes measured with board square unit
			unsigned width, height;
			unsigned number_of_squares;

			std::vector<SquareState> squares_states;
		};
	public:
		XOGameLogic(unsigned initial_width, unsigned initial_height, PlayerSymbol initial_player, unsigned initial_winning_streak);

		bool play_move(unsigned x, unsigned y);
		// void restart_game(unsigned board_width, unsigned board_height, PlayerSymbol starting_player);

		PlayerSymbol player_to_play_next();
		PlayerSymbol winner();
		GameState current_state();

		// prints text representation of the game
		void print_status();
	private:
		BoardInfo _board_info;
		PlayerSymbol _current_player;
		GameState _current_state;
		std::vector<std::vector<unsigned>> _winning_squares_sqruences;
		unsigned _winning_streak;

		void _analyse_board();
		PlayerSymbol _change_player();
	};
}
