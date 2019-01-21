#pragma once
#include "Memento.h"
#include "BoardInfo.h"

namespace xo
{
	class XOGameLogicMemento : public Memento
	{
	public:
		void set_state(const BoardInfo& board_info, PlayerSymbol current_player, GameState current_state, const std::vector<std::vector<unsigned>> &winning_squares_sqruences, unsigned winning_streak);
		void get_state(BoardInfo *board_info, PlayerSymbol *current_player, GameState *current_state, std::vector<std::vector<unsigned>> *winning_squares_sqruences, unsigned *winning_streak);

		void save(std::ostream &) override;
		void load(std::istream &) override;

		GameState state();

	private:
		BoardInfo _board_info;
		PlayerSymbol _current_player;
		GameState _current_state;
		std::vector<std::vector<unsigned>> _winning_squares_sqruences;
		unsigned _winning_streak;
	};
}