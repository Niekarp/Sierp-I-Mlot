#include "pch.h"
#include "XOGameLogicMemento.h"

using namespace xo;

void xo::XOGameLogicMemento::set_state(const BoardInfo & board_info, PlayerSymbol current_player, GameState current_state, const std::vector<std::vector<unsigned>>& winning_squares_sqruences, unsigned winning_streak)
{
	_board_info = board_info;
	_current_player = current_player;
	_current_state = current_state;
	_winning_squares_sqruences = winning_squares_sqruences;
	_winning_streak = winning_streak;
}

void xo::XOGameLogicMemento::get_state(BoardInfo * board_info, PlayerSymbol * current_player, GameState * current_state, std::vector<std::vector<unsigned>>* winning_squares_sqruences, unsigned * winning_streak)
{
	*board_info = _board_info;
	*current_player = _current_player;
	*current_state = _current_state;
	*winning_squares_sqruences = _winning_squares_sqruences;
	*winning_streak = _winning_streak;
}

void xo::XOGameLogicMemento::save(std::ostream &out)
{
	// _board_info
	out << _board_info.height << ' ' << _board_info.number_of_squares << ' ';
	out << _board_info.squares_states.size() << ' ';
	for (auto ps : _board_info.squares_states) out << (int)ps << ' ';
	out << _board_info.width << ' ';

	// _current_player
	out << (int)_current_player << ' ';

	// _current_state
	out << (int)_current_state << ' ';

	// _winning_squares_sqruences
	out << _winning_squares_sqruences.size() << ' ';
	for (auto ws : _winning_squares_sqruences) {
		out << ws.size() << ' ';
		for (auto wss : ws) {
			out << wss << ' ';
		}
	}

	out << _winning_streak << ' ';
}

void xo::XOGameLogicMemento::load(std::istream &in)
{
	int tmp;

	// _board_info
	in >> _board_info.height >> _board_info.number_of_squares;
	size_t squares_states_count = 0;
	in >> squares_states_count;
	_board_info.squares_states.reserve(squares_states_count);
	for (auto &ss : _board_info.squares_states) {
		in >> tmp;
		ss = static_cast<SquareState>(tmp);
	}
	in >> _board_info.width;

	// _current_player
	in >> tmp;
	_current_player = static_cast<PlayerSymbol>(tmp);

	// _current_state
	in >> tmp;
	_current_state = static_cast<GameState>(tmp);

	// _wining_squares_sqruences
	in >> tmp;
	_winning_squares_sqruences.resize(tmp);
	for (auto &ws : _winning_squares_sqruences) {
		in >> tmp;
		ws.resize(tmp);
		for (auto &wss : ws) {
			in >> wss;
		}
	}

	in >> _winning_streak;
}

GameState xo::XOGameLogicMemento::state()
{
	return _current_state;
}
