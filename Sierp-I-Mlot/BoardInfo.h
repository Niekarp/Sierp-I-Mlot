#pragma once
#include "PlayerSymbol.h"

namespace xo
{
	typedef PlayerSymbol SquareState;

	enum class GameState
	{
		ongoing,
		stopped,
		finished
	};

	struct BoardInfo
	{
		// sizes measured with board square unit
		unsigned width, height;
		unsigned number_of_squares;

		std::vector<SquareState> squares_states;
	};
}