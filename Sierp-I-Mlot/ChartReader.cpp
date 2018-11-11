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
	std::string line;

	while (std::getline(input_file, line))
	{
		if (line == "[ExpertSingle]")
		{
			std::getline(input_file, line);
			break;
		}
	}

	while (input_file)
	{
		XOIGameMapHero::Note note;

		input_file >> note.time;
		std::getline(input_file, line, '=');
		char symbol;
		input_file >> symbol;

		if (symbol != 'N')
		{
			std::getline(input_file, line);
			continue;
		}

		input_file >> note.tone;
		input_file >> note.duration;

		std::getline(input_file, line);

		if (note.tone > 3)
		{
			continue;
		}

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
