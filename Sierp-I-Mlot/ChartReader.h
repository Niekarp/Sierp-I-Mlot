#pragma once
#include "Note.h"

struct ChartReader
{
	ChartReader(const char *filename);
	void load(const char *filename);
	std::vector<Note> notes();

private:
	std::vector<Note> _notes;
};

