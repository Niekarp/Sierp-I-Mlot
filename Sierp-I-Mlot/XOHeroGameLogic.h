#pragma once
#include "XOIGameMapHero.h"
#include "MusicPlayer.h"
#include "ChartReader.h"
#include "NoteContainer.h"

namespace xo
{
	class XOHeroGameLogic
	{
	public:
		XOHeroGameLogic();

		void turn(int);
		std::shared_ptr<NoteContainer> load(const std::string &filename);
		void start(long long start_time, long long omitt);
		void on_note_omitted(const std::function<void(const Note &)> &);
		Note *button_pressed(int button);
		float progress();
		void reset();
		bool won();
		void destroy_note(Note *);

	private:
		size_t _find_nearest_note_index(std::chrono::milliseconds ms);
		void _feed_with_next_notes();
		void _on_note_omitted(const Note &);
		void _add_points(int points);

		std::vector<std::function<void(const Note&)>> _omitted_callbacks;

		std::shared_ptr<NoteContainer> _notes;
		ChartReader _chart_reader;
		int _turn;
		size_t _last_nearest_note_index;
		std::chrono::time_point<std::chrono::system_clock> _start_point;
		HANDLE _queue_timer;
		int _current_progress;
		long long _ommision_time;
		long long _start_time;
		bool _play;
		bool _start;
	};
}