#pragma once
struct MusicPlayer
{
	void load(const std::string &filename);
	void play();
	void stop();

private:
	std::string _filename;
};

