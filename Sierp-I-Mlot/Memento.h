#pragma once

namespace xo
{
	class Memento
	{
		friend class MementoFileSerializer;

	protected:
		virtual void save(std::ostream& out) = 0;
		virtual void load(std::istream& in) = 0;
	};

	class MementoFileSerializer
	{
		Memento &_memento;
	public:
		MementoFileSerializer(Memento &memento) : _memento(memento) {}
		void save(const char *path)
		{
			std::fstream out(path, std::ios::out);
			_memento.save(out);
		}
		void load(const char *path)
		{
			std::fstream in(path, std::ios::in);
			_memento.load(in);
		}
	};
}