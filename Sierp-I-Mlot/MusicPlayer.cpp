#include "pch.h"
#include "MusicPlayer.h"

using namespace xo;

xo::MusicPlayer::MusicPlayer() :
	_begin_queue_timer(NULL),
	_end_queue_timer(NULL),
	_from(0),
	_to(-1),
	_stopped(true)
{
	CHKERR_HANDLE(_thread_event = CreateEvent(NULL, FALSE, FALSE, NULL));
	CHKERR_HANDLE(_commands_mutex = CreateMutex(NULL, FALSE, NULL));

	_thread = CreateThread(NULL, 0, [](void *this_) -> DWORD
	{

		struct {
			MusicPlayer *mp;

			void load()
			{
				stop();

				auto device_hash = std::hash<std::string>{}(mp->_filename);

				mp->_alias = (std::stringstream() << "xoMusicPlayer" << device_hash).str();

				if (std::find(mp->_devices.begin(), mp->_devices.end(), device_hash) == mp->_devices.end())
				{
					OutputDebugStringA((std::stringstream() << "<< MUSIC:LOAD : " << mp->_alias << " >>\n").str().c_str());

					std::stringstream ss;
					ss << "open waveaudio!" << mp->_filename << "  alias " << mp->_alias;

					CHKERR_MCI(mciSendStringA(ss.str().c_str(), NULL, 0, 0));

					ss.str("");
					ss << "set " << mp->_alias << " time format milliseconds";

					CHKERR_MCI(mciSendStringA(ss.str().c_str(), NULL, 0, 0));

					mp->_devices.push_back(device_hash);
				}
			}

			void play()
			{
				OutputDebugStringA("cmd_proc:play\n");
				stop();

				OutputDebugStringA((std::stringstream() << "<< MUSIC:PLAY : " << mp->_alias << " >>\n").str().c_str());
				std::stringstream ss;
				ss << "play " << mp->_alias << " from " << mp->_from;

				if (mp->_to != -1)
				{
					ss << " to " << mp->_to;
				}

				CHKERR_MCI(mciSendStringA(ss.str().c_str(), NULL, 0, 0));
				mp->_stopped = false;

				if (mp->_play_callback)
				{
					mp->_play_callback();
				}
			}

			void stop()
			{
				OutputDebugStringA((std::stringstream() << "<< MUSIC:STOP : " << mp->_alias << " >>\n").str().c_str());
				std::stringstream ss;
				ss << "stop " << mp->_alias;

				mciSendStringA(ss.str().c_str(), NULL, 0, 0);
				mp->_stopped = true;
			}
		} cmd_proc{ (MusicPlayer *)this_ };


		for (;;)
		{
			CHKERR_DWORD(WaitForSingleObject(cmd_proc.mp->_thread_event, INFINITE));

			OutputDebugStringA("[cmd_proc: recived]\m");

			for (;;)
			{
				CHKERR_DWORD(WaitForSingleObject(cmd_proc.mp->_commands_mutex, INFINITE));
				if (cmd_proc.mp->_commands.empty())
				{
					CHKERR_MUTEX(ReleaseMutex(cmd_proc.mp->_commands_mutex));
					break;
				}
				auto next_command = cmd_proc.mp->_commands.back();
				cmd_proc.mp->_commands.pop_back();
				CHKERR_MUTEX(ReleaseMutex(cmd_proc.mp->_commands_mutex));

				switch (next_command)
				{

				case Command::LOAD:
					cmd_proc.load();
					break;

				case Command::PLAY:
					cmd_proc.play();
					break;

				case Command::STOP:
					cmd_proc.stop();
					break;

				}
			}
		}

	}, this, 0, NULL);

	CHKERR_HANDLE(_thread);
}

void xo::MusicPlayer::load(const std::string & filename)
{
	CHKERR_DWORD(WaitForSingleObject(_commands_mutex, INFINITE));
	_commands.push_front(Command::LOAD);
	_filename = filename;
	CHKERR_MUTEX(ReleaseMutex(_commands_mutex));
	SetEvent(_thread_event);
}

void MusicPlayer::play(int from, int to, int at)
{
	_from = from;
	_to = to;
	_at = at;

	if (at == 0)
	{
		_play(from, to);
		return;
	}

	if (!_executor)
	{
		throw std::exception("_executor == null");
	}

	if (_begin_queue_timer)
	{
		CHKERR_DWORD_DeleteTimerQueueTimer(DeleteTimerQueueTimer(NULL, _begin_queue_timer, NULL));
	}

	if (_end_queue_timer)
	{
		CHKERR_DWORD_DeleteTimerQueueTimer(DeleteTimerQueueTimer(NULL, _end_queue_timer, NULL));
	}

	CHKERR_BOOL(CreateTimerQueueTimer(&_begin_queue_timer, NULL, [](PVOID music_player_, BOOLEAN towf)
	{
		auto music_player = static_cast<MusicPlayer *>(music_player_);
		music_player->_executor->execute([music_player] {
			music_player->_play(music_player->_from, music_player->_to);
		});
	}, this, at, 0, WT_EXECUTEDEFAULT));

	if (_end_callback)
	{
		CHKERR_BOOL(CreateTimerQueueTimer(&_end_queue_timer, NULL, [](PVOID music_player_, BOOLEAN towf)
		{
			auto music_player = static_cast<MusicPlayer *>(music_player_);
			music_player->_end_callback();
		}, this, at + to - from, 0, WT_EXECUTEDEFAULT));
	}
}

void MusicPlayer::_play(int from, int to)
{
	OutputDebugStringA("MusicPlayer:play\n");

	CHKERR_DWORD(WaitForSingleObject(_commands_mutex, INFINITE));
	_commands.push_front(Command::PLAY);
	_from = from;
	_to = to;
	CHKERR_MUTEX(ReleaseMutex(_commands_mutex));
	SetEvent(_thread_event);
}

void MusicPlayer::stop()
{
	if (_stopped)
	{
		return;
	}

	CHKERR_DWORD(WaitForSingleObject(_commands_mutex, INFINITE));
	_commands.push_front(Command::STOP);
	CHKERR_MUTEX(ReleaseMutex(_commands_mutex));
	SetEvent(_thread_event);
}

void xo::MusicPlayer::delegate_executor(const std::shared_ptr<XOIDelegateExecutor> &executor)
{
	_executor = executor;
}

void xo::MusicPlayer::play_event(const std::function<void()>& callback)
{
	_play_callback = callback;
}

void xo::MusicPlayer::end_event(const std::function<void()>& callback)
{
	_end_callback = callback;
}
