#pragma once

#include <utility>
#include "PlayerSymbol.h"
#include "Memento.h"

namespace xo
{
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

		class GameLogicMemento :
			public Memento
		{
			BoardInfo _board_info;
			PlayerSymbol _current_player;
			GameState _current_state;
			std::vector<std::vector<unsigned>> _winning_squares_sqruences;
			unsigned _winning_streak;

		protected:
			void save(std::ostream& out);
			void load(std::istream& in);

		public:
			void set_state(const BoardInfo &board_info,
				const PlayerSymbol &current_player,
				const GameState &current_state,
				const std::vector<std::vector<unsigned>> &winning_squares_sqruences,
				const unsigned &winning_streak);

			void get_state(BoardInfo &board_info,
				PlayerSymbol &current_player,
				GameState &current_state,
				std::vector<std::vector<unsigned>> &winning_squares_sqruences,
				unsigned &winning_streak);
		};

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
		std::shared_ptr<Memento> create_memeto();
		std::shared_ptr<Memento> create_empty_memeto();
		void set_memento(const std::shared_ptr<Memento> &);
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
