#pragma once

#include <utility>

namespace xo
{
	typedef std::pair<unsigned, unsigned> BoardSize;
	typedef std::pair<unsigned, unsigned> BoardPosition;
	enum class PlayerSymbol
	{
		circle,
		cross
	};


	class XOGameLogic
	{
	public:
		XOGameLogic(BoardSize initial_size);

		bool make_move(BoardPosition, PlayerSymbol);
		bool make_move(unsigned, PlayerSymbol);

		PlayerSymbol get_current_player();
		BoardSize get_board_size();
		unsigned get_number_of_fields();
	private:
		BoardSize _board_size;
	};
}
