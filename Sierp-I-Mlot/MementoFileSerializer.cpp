#include "pch.h"
#include "MementoFileSerializer.h"

xo::MementoFileSerializer::MementoFileSerializer(const std::shared_ptr<Memento> &memento) :
	_memento(memento)
{
}

void xo::MementoFileSerializer::save(const std::string &path)
{
	std::ofstream out(path);
	_memento->save(out);
}

void xo::MementoFileSerializer::load(const std::string &path)
{
	std::ifstream in(path);
	if(in.is_open())
		_memento->load(in);
}
