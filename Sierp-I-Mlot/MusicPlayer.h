#pragma once
#include "XOIDelegateExecutor.h"

namespace xo
{
	struct MusicPlayer
	{
		MusicPlayer();
		void load(const std::string &filename);
		void play(int from = 0, int to = -1, int at = 0);
		void stop();
		void delegate_executor(const std::shared_ptr<XOIDelegateExecutor> &);
		void play_event(const std::function<void()> &callback);
		void end_event(const std::function<void()> &callback);

	private:
		enum class Command
		{
			NONE,
			LOAD,
			PLAY,
			STOP
		};

		void _play(int from, int to);

		int _from;
		int _to;
		int _at;
		HANDLE _begin_queue_timer;
		HANDLE _end_queue_timer;
		std::string _filename;
		std::shared_ptr<XOIDelegateExecutor> _executor;
		std::function<void()> _play_callback;
		std::function<void()> _end_callback;
		std::vector<size_t> _devices;
		std::string _alias;
		bool _stopped;
		HANDLE _thread;
		HANDLE _thread_event;
		HANDLE _commands_mutex;

		std::deque<Command> _commands;
	};
}