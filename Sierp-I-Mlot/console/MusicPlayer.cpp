#include "../pch.h"
#include "MusicPlayer.h"

void MusicPlayer::load(const char * filename)
{
	_filename = filename;
}

void MusicPlayer::play()
{
	PlaySoundA(_filename, NULL, SND_ASYNC);
}

void MusicPlayer::stop()
{
	PlaySoundA(NULL, NULL, 0);
}
