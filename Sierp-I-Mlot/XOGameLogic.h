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
		XOGameLogic(unsigned initial_width, unsigned initial_height);

		bool play_move(PlayerSymbol symbol, unsigned x, unsigned y);
		void analyse_board();

		PlayerSymbol player_to_play_next();
		GameState current_state();
	private:
		BoardInfo _board_info;
		PlayerSymbol _current_player;
		GameState _current_state;

		std::vector<std::vector<unsigned>> _winning_squares_sqruences;
	};
}
