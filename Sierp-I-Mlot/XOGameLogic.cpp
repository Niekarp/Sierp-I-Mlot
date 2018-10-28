#include "pch.h"
#include "XOGameLogic.h"

namespace xo
{
	XOGameLogic::XOGameLogic(unsigned initial_width, unsigned initial_height, PlayerSymbol initial_player)
	{
		_board_info.width = initial_width;
		_board_info.height = initial_height;
		_board_info.number_of_squares = initial_width * initial_height;

		_board_info.squares_states.reserve(_board_info.number_of_squares);
		for (int i = 0; i < _board_info.number_of_squares; ++i)
		{
			_board_info.squares_states.emplace(
				_board_info.squares_states.begin() + i, SquareState::none);
		}


		auto& seq = _winning_squares_sqruences;
		unsigned winning_lines = _board_info.height + _board_info.width;
		if (_board_info.height == _board_info.width) winning_lines += 2;

		for (int i = 0; i < _board_info.height; ++i)
		{
			seq.emplace_back();
			for (int j = 0; j < _board_info.width; ++j)
			{
				seq[i].emplace_back();
			}
		}
		auto sum = _board_info.height + _board_info.width;
		for (int i = _board_info.height; i < sum; ++i)
		{
			seq.emplace_back();
			for (int j = 0; j < _board_info.height; ++j)
			{
				seq[i].emplace_back();
			}
		}
		if (_board_info.height == _board_info.width)
		{
			for (int i = sum; i < (sum + 2); ++i)
			{
				seq.emplace_back();
				for (int j = 0; j < _board_info.height; ++j)
				{
					seq[i].emplace_back();
				}
			}
		}

		for (int x = 0; x < _board_info.height; ++x) {
			for (int y = 0; y < seq[x].size(); ++y) {
				unsigned index = y + (x * _board_info.width);

				seq[x][y] = index;
				seq[_board_info.height + y][x] = index;

				if (_board_info.height == _board_info.width) {
					if (x == y)
					{
						seq[winning_lines - 2][y] = index;
					}
					if (x + y + 1 == _board_info.height)
					{
						seq[winning_lines - 1][x] = index;
					}
				}
			}
		}

		_current_player = initial_player;
		_current_state = GameState::ongoing;
	}

	bool XOGameLogic::play_move(unsigned x, unsigned y)
	{
		auto square_index = x + (y * _board_info.height);
		if (_board_info.squares_states.at(square_index) == SquareState::none)
		{
			_board_info.squares_states.at(square_index) = _current_player;
			return true;
		}
		return false;
	}

	void XOGameLogic::_analyse_board()
	{
		SquareState field, last_field;

		unsigned winning_lines = _board_info.height + _board_info.width;
		unsigned amount_of_the_same_fields;
		auto& seq = _winning_squares_sqruences;

		auto _winning_streak = 3;

		for (int i = 0; i < winning_lines; ++i)
		{
			field = last_field = _board_info.squares_states[seq[i][0]];
			amount_of_the_same_fields = 0;
			unsigned winning_streak = _winning_streak;

			for (int j = 0; j < seq[i].size(); ++j)
			{
				if (_winning_streak == -1) winning_streak = seq[i].size();

				field = _board_info.squares_states[seq[i][j]];
				if (field != last_field)
				{
					last_field = field;
					amount_of_the_same_fields = 1;
				}
				else
				{
					++amount_of_the_same_fields;
					if (amount_of_the_same_fields == winning_streak && field != SquareState::none)
					{
						_current_state = GameState::finished;
						return;
					}
				}
			}
		}

		for (auto e : _board_info.squares_states) {
			if (e == SquareState::none) return;
		}
		_current_state = GameState::finished;
	}

	PlayerSymbol XOGameLogic::player_to_play_next()
	{
		return _current_player;
	}
	PlayerSymbol XOGameLogic::winner()
	{
		if (_current_state == GameState::finished)
		{
			if (_current_player == PlayerSymbol::circle) return PlayerSymbol::cross;
			else return PlayerSymbol::circle;
		}
		return PlayerSymbol::none;
	}
	GameState XOGameLogic::current_state()
	{
		return _current_state;
	}
}
