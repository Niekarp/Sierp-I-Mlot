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

void xo::NoteContainer::clear()
{
	_notes.clear();
}

NoteContainer::iterator xo::NoteContainer::begin()
{
	auto shr = shared_from_this();
	return iterator(shared_from_this(), 0);
}

NoteContainer::iterator xo::NoteContainer::end()
{
	return iterator(shared_from_this(), _notes.size());
}
