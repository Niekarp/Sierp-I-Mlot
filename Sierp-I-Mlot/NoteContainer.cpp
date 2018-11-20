#include "pch.h"
#include "NoteContainer.h"

using namespace xo;

NoteContainer::NoteContainer() : 
	_arranged(true)
{
}

void NoteContainer::push(const Note &note)
{
	_notes.push_back(note);
	_arranged = false;
}

void NoteContainer::arrange()
{
	std::sort(_notes.begin(), _notes.end(), [](auto &a, auto &b) {return a.time < b.time; });
	_arranged = true;
}

Note &NoteContainer::operator[](size_t idx)
{
	return at(idx);
}

Note & xo::NoteContainer::at(size_t idx)
{
	if (!_arranged)
	{
		arrange();
	}
	return _notes[idx];
}

size_t NoteContainer::size()
{
	return _notes.size();
}

void xo::NoteContainer::clear()
{
	_notes.clear();
}
