#pragma once

#include <utility>
#include "PlayerSymbol.h"
#include "BoardInfo.h"
#include "XOGameLogicMemento.h"

namespace xo
{
	class XOGameLogic
	{
	public:
		XOGameLogic(unsigned initial_width, unsigned initial_height, PlayerSymbol initial_player, unsigned initial_winning_streak);

		bool play_move(unsigned x, unsigned y);
		bool peek_move(unsigned x, unsigned y);
		void restart_game(unsigned board_width, unsigned board_height, PlayerSymbol starting_player, unsigned initial_winning_streak);

		PlayerSymbol player_to_play_next();
		PlayerSymbol winner();
		GameState current_state();
		void pass_turn();

		// prints text representation of the game
		void print_status();

		// memento
		std::shared_ptr<Memento> create_memento();
		void set_memento(const std::shared_ptr<Memento> &);
		void for_each_square(const std::function<void(int x, int y, SquareState state)> &callback);

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
