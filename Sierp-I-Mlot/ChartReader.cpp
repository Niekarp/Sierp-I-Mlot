#include "pch.h"
#include "ChartReader.h"

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
		Note note;

		input_file >> note.timestamp;
		std::getline(input_file, line, '=');
		char symbol;
		input_file >> symbol;

		if (symbol != 'N')
		{
			std::getline(input_file, line);
			continue;
		}

		input_file >> note.key;
		input_file >> note.duration;

		std::getline(input_file, line);

		_notes.push_back(note);
	}
}

std::vector<Note> ChartReader::notes()
{
	return _notes;
}
