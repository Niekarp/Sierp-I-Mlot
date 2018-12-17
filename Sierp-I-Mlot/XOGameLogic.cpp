#include "pch.h"
#include "XOGameLogic.h"

namespace xo
{
	std::string _player_symbol_to_string(PlayerSymbol player_symbol);
	std::string _game_state_to_string(GameState game_state);

	XOGameLogic::XOGameLogic(unsigned initial_width, unsigned initial_height, PlayerSymbol initial_player, unsigned initial_winning_streak)
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


		auto &seq = _winning_squares_sqruences;
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

		_winning_streak = initial_winning_streak;
	}

	bool XOGameLogic::play_move(unsigned x, unsigned y)
	{
		/*std::cout << "\t\tplaying a move: [" << x << "][" << y << "] as " << _player_symbol_to_string(_current_player) << std::endl;
		std::wstring ws = L"\t\tplaying a move: [" + std::to_wstring(x) + L"][" + std::to_wstring(y) + L"] as " + std::to_wstring(static_cast<int>(_current_player)) + L"\n";

		MessageBox(0, ws.c_str(), NULL, MB_OK);*/

		auto square_index = x + (y * _board_info.height);
		if (_board_info.squares_states.at(square_index) == SquareState::none)
		{
			_board_info.squares_states.at(square_index) = _current_player;
			this->_change_player();
			_analyse_board();
			return true;
		}
		return false;
	}

	bool XOGameLogic::peek_move(unsigned x, unsigned y)
	{
		auto square_index = x + (y * _board_info.height);
		if (_board_info.squares_states.at(square_index) == SquareState::none)
		{
			return true;
		}
		return false;
	}

	void xo::XOGameLogic::restart_game(unsigned board_width, unsigned board_height, PlayerSymbol starting_player, unsigned initial_winning_streak)
	{
		XOGameLogic new_game = XOGameLogic(board_width, board_height, starting_player, initial_winning_streak);

		*(this) = new_game;
	}

	void XOGameLogic::_analyse_board()
	{
		SquareState field, last_field;

		unsigned winning_lines = _board_info.height + _board_info.width;
		if (_board_info.width == _board_info.height)
		{
			winning_lines += 2;
		}
		unsigned amount_of_the_same_fields;
		auto &seq = _winning_squares_sqruences;

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
						std::wstring plr;
						if (_current_player == xo::PlayerSymbol::circle)
						{
							plr = L"krzy¿ek";
						}
						else
						{
							plr = L"kó³ek";
						}

						//std::wstring ws = L"Omaj " + plr +  L" wygra³!\nTa gra dzia³a!!";
						//MessageBox(0, ws.c_str(), NULL, MB_OK);
						this->_change_player();
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
			return _current_player;
		}
		return PlayerSymbol::none;
	}

	GameState XOGameLogic::current_state()
	{
		return _current_state;
	}

	void XOGameLogic::pass_turn()
	{
		_change_player();
	}

	PlayerSymbol XOGameLogic::_change_player()
	{
		if (_current_player == PlayerSymbol::circle)
		{
			_current_player = PlayerSymbol::cross;
		}
		else
		{
			_current_player = PlayerSymbol::circle;
		}
		return _current_player;
	}

	std::string _player_symbol_to_string(PlayerSymbol player_symbol)
	{
		if (player_symbol == PlayerSymbol::cross)
		{
			return "X";
		}
		else if (player_symbol == PlayerSymbol::circle)
		{
			return "O";
		}
		return "none";
	}

	std::string _game_state_to_string(GameState game_state)
	{
		if (game_state == GameState::ongoing)
		{
			return "ongoing";
		}
		else if (game_state == GameState::stopped)
		{
			return "stopped";
		}
		return "finished";
	}

	using namespace std;
	void XOGameLogic::print_status()
	{
		cout << "Board info" << endl
			<< "width: " << _board_info.width << endl
			<< "height: " << _board_info.height << endl
			<< "number of squares: " << _board_info.number_of_squares << endl
			<< "squares states: " << endl;
		for (int i = 0; i < _board_info.squares_states.size(); ++i)
		{
			auto e = _board_info.squares_states.at(i);
			cout << "[" << _player_symbol_to_string(e) << "] ";
			if (((i % _board_info.width) / (_board_info.height - 1)) == 1)
			{
				cout << endl;
			}
		}
		cout << endl;

		cout << "Player to play next: " << _player_symbol_to_string(_current_player) << endl
			<< "Winner: " << _player_symbol_to_string(this->winner()) << endl
			<< "Current state: " << _game_state_to_string(_current_state) << endl;

		cout << "Winning squares sequences: ";
		for (auto e : _winning_squares_sqruences)
		{
			cout << endl;
			for (auto ee : e)
			{
				cout << "	[" << ee << "] ";
			}
		}
		cout << endl << endl;
	}

	//memento

	std::shared_ptr<Memento> XOGameLogic::create_memeto()
	{
		auto memento = std::make_shared<GameLogicMemento>();
		memento->set_state(_board_info, _current_player, _current_state, _winning_squares_sqruences, _winning_streak);
		return memento;
	}
	std::shared_ptr<Memento> XOGameLogic::create_empty_memeto()
	{
		return std::make_shared<GameLogicMemento>();
	}
	void XOGameLogic::set_memento(const std::shared_ptr<Memento>& memento)
	{
		auto mem = std::dynamic_pointer_cast<GameLogicMemento>(memento);
		mem->get_state(_board_info, _current_player, _current_state, _winning_squares_sqruences, _winning_streak);
	}

	void XOGameLogic::GameLogicMemento::save(std::ostream& out)
	{
		out << _board_info.width << " "
			<< _board_info.height << " "
			<< _board_info.number_of_squares << " "
			<< _board_info.squares_states.size() << " ";
		for (auto &ss : _board_info.squares_states)
			out << static_cast<int>(ss) << " ";
		out << static_cast<int>(_current_player) << " "
			<< static_cast<int>(_current_state) << " "
			<< _winning_squares_sqruences.size() << " "
			<< _winning_squares_sqruences[0].size() << " ";
		for (auto &wss : _winning_squares_sqruences) for (auto &ws : wss)
			out << ws << " ";
		out << _winning_streak;

	}
	void XOGameLogic::GameLogicMemento::load(std::istream& in)
	{
		size_t board_info_squares_states_sz;
		std::underlying_type<PlayerSymbol>::type current_player;
		std::underlying_type<GameState>::type current_state;
		size_t winning_squares_sqruences_sz;
		size_t winning_squares_sqruences_sz2;

		in >> _board_info.width
			>> _board_info.height
			>> _board_info.number_of_squares
			>> board_info_squares_states_sz;
		for (size_t i = 0; i < board_info_squares_states_sz; ++i)
		{
			std::underlying_type<SquareState>::type v; in >> v;
			_board_info.squares_states.push_back(static_cast<SquareState>(v));
		}
		in >> current_player
			>> current_state
			>> winning_squares_sqruences_sz
			>> winning_squares_sqruences_sz2;
		for (int i = 0; i < winning_squares_sqruences_sz; ++i)
		{
			_winning_squares_sqruences.emplace_back();
			for (int j = 0; j < winning_squares_sqruences_sz2; ++j)
			{
				unsigned v;
				in >> v;
				_winning_squares_sqruences.back().push_back(v);
			}
		}
		in >> _winning_streak;
	}

	void XOGameLogic::GameLogicMemento::set_state(const BoardInfo &board_info,
		const PlayerSymbol &current_player,
		const GameState &current_state,
		const std::vector<std::vector<unsigned>> &winning_squares_sqruences,
		const unsigned &winning_streak)
	{
		_board_info = board_info;
		_current_player = current_player;
		_current_state = current_state;
		_winning_squares_sqruences = winning_squares_sqruences;
		_winning_streak = winning_streak;
	}

	void XOGameLogic::GameLogicMemento::get_state(BoardInfo &board_info,
		PlayerSymbol &current_player,
		GameState &current_state,
		std::vector<std::vector<unsigned>> &winning_squares_sqruences,
		unsigned &winning_streak)
	{
		board_info = _board_info;
		current_player = _current_player;
		current_state = _current_state;
		winning_squares_sqruences = _winning_squares_sqruences;
		winning_streak = _winning_streak;
	}
}
