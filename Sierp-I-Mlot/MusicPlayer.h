#pragma once
struct MusicPlayer
{
	void load(const char *filename);
	void play();
	void stop();

private:
	const char *_filename;
};

