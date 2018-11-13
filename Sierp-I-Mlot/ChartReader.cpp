#include "pch.h"
#include "ChartReader.h"

using namespace xo;

ChartReader::ChartReader(const char * filename)
{
	load(filename);
}

void ChartReader::load(const char * filename)
{
	std::fstream input_file(filename, std::ios::in);
	XOIGameMapHero::Note note;
	while (input_file >> note.tone >> note.time) 
	{
		note.tone -= 1;
		_notes.push_back(note);
	}
}

void xo::ChartReader::put_notes_on(const std::shared_ptr<XOIGameMapHero>& game_map)
{
	for (auto &note : _notes)
	{
		game_map->put(note);
	}
}
