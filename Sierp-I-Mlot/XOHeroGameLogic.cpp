#include "pch.h"
#include "XOHeroGameLogic.h"
#include "ChartReader.h"

using namespace xo;

XOHeroGameLogic::XOHeroGameLogic() :
	_turn(0),
	_chart_reader(),
	_current_progress(50),
	_ommision_time(0),
	_queue_timer(0),
	_play(false),
	_start(false)
{
}

void XOHeroGameLogic::turn(int turn)
{
	_turn = turn;
}

std::shared_ptr<NoteContainer> XOHeroGameLogic::load(const std::string &filename)
{
	_chart_reader.load(filename);
	_notes = _chart_reader.notes();
	return _notes;

	/*map->start(0);
	_music_player->stop();
	_music_player->load(conf::FILE_MUSIC_GAME_HERO);
	_music_player->play();
	*/
}

void xo::XOHeroGameLogic::start(long long start_time, long long wait)
{
	OutputDebugStringA("XOHeroGameLogic: start");
	_play = true;
	_ommision_time = wait;
	_start_point = std::chrono::system_clock::now();
	CHKERR_BOOL(CreateTimerQueueTimer(&_queue_timer, NULL, [](PVOID logic_, BOOLEAN towf)
	{
		auto logic = (XOHeroGameLogic *)logic_;
		logic->_feed_with_next_notes();

	}, this, wait, 0, WT_EXECUTEDEFAULT));
}

void XOHeroGameLogic::on_note_omitted(const std::function<void(const Note&)>& callback)
{
	_omitted_callbacks.push_back(callback);
}

Note *XOHeroGameLogic::button_pressed(int button)
{
	if (!_play)
	{
		_add_points(conf::GAME_HERO_PROGRESS_PENALTY_WRONG_CLICK);
		return nullptr;
	}
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - _start_point);
	if (duration.count() <= _ommision_time)
	{
		return nullptr;
	}

	for (auto &note : *_notes)
	{
		if (note.time >= duration.count() + conf::GAME_HERO_LOGIC_NOTE_TOLERANCE)
		{
			break;
		}

		if (note.clicked)
		{
			continue;
		}

		if (note.tone == button)
		{
			_add_points(conf::GAME_HERO_PROGRESS_REWARD_GOOD_CLICK);
			note.clicked = true;
			return &note;
		}
	}

	_add_points(conf::GAME_HERO_PROGRESS_PENALTY_WRONG_CLICK);

	return nullptr;
}

float XOHeroGameLogic::progress()
{
	return 1.f * _current_progress / conf::GAME_HERO_PROGRESS_MAX;
}

void xo::XOHeroGameLogic::reset()
{
	OutputDebugStringA("XOHeroGameLogic: reset");
	_play = false;
	_current_progress = 50;
}

bool xo::XOHeroGameLogic::won()
{
	return !(progress() < 0.75f);
}

void xo::XOHeroGameLogic::destroy_note(Note *note)
{
	if (note == nullptr)
	{
		return;
	}

	auto now = std::chrono::system_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - _start_point);
	note->destroy_time = duration.count();
}

size_t XOHeroGameLogic::_find_nearest_note_index(std::chrono::milliseconds now)
{
	/*auto i = _last_nearest_note_index;
	for (; i < _notes.size(); ++i)
	{
		if (_notes[i].time >= now.count())
		{
			break;
		}
	}*/
	return 0; // TODO
}

void XOHeroGameLogic::_feed_with_next_notes()
{
	if (!_play)
	{
		if (_queue_timer)
		{
			CHKERR_DWORD_DeleteTimerQueueTimer(DeleteTimerQueueTimer(NULL, _queue_timer, NULL));
			_queue_timer = NULL;
		}

		return;
	}

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - _start_point);


	auto it = _notes->begin(), end_it = _notes->end();
	for (; it != end_it; ++it)
	{
		if ((*it).time < _ommision_time)
		{
			continue;
		}

		if ((*it).time >= duration.count() - conf::GAME_HERO_LOGIC_NOTE_TOLERANCE)
		{
			break;
		}

		if ((*it).clicked)
		{
			continue;
		}

		(*it).clicked = true;

		_on_note_omitted((*it));
	}

	/*if (_last_nearest_note_index >= _notes->size())
	{
		return;
	}*/
	if (it == end_it)
	{
		return;
	}

	auto _next_callback_time = (*it).time - duration.count();

	if (_next_callback_time < 0)
	{
		_next_callback_time += conf::GAME_HERO_LOGIC_NOTE_TOLERANCE;
	}
	OutputDebugStringA((std::stringstream() << this << " work\n").str().c_str());
	CHKERR_DWORD_DeleteTimerQueueTimer(DeleteTimerQueueTimer(NULL, _queue_timer, NULL));
	_queue_timer = NULL;
	CHKERR_BOOL(CreateTimerQueueTimer(&_queue_timer, NULL, [](PVOID logic_, BOOLEAN towf)
	{
		auto logic = (XOHeroGameLogic *)logic_;
		logic->_feed_with_next_notes();
	}, this, _next_callback_time, 0, WT_EXECUTEDEFAULT));
}

void XOHeroGameLogic::_on_note_omitted(const Note &note)
{
	_add_points(conf::GAME_HERO_PROGRESS_PENALTY_DROP);
	for (auto &callback : _omitted_callbacks)
	{
		callback(note);
	}
}

void xo::XOHeroGameLogic::_add_points(int points)
{
	_current_progress = max(min(_current_progress + points, 100), 0);
}
