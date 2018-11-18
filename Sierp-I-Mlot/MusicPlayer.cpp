#include "pch.h"
#include "MusicPlayer.h"

void MusicPlayer::load(const std::string & filename)
{
	_filename = filename;
}

void MusicPlayer::play()
{
	PlaySoundA(_filename.c_str(), NULL, SND_ASYNC | SND_LOOP);
}

void MusicPlayer::stop()
{
	PlaySoundA(NULL, NULL, 0);
}
