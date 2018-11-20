#include "pch.h"
#include "ChartReader.h"

using namespace xo;

ChartReader::ChartReader()
{
	_notes = std::make_shared<NoteContainer>();
}

void ChartReader::load(const std::string & filename)
{
	_notes->clear();
	std::fstream input_file(filename, std::ios::in);
	Note note{};
	while (input_file >> note.tone >> note.time) 
	{
		note.destroy_time = -1;
		note.tone -= 1;
		_notes->push(note);
	}
}

std::shared_ptr<NoteContainer> xo::ChartReader::notes()
{
	_notes->arrange();
	return _notes;
}
