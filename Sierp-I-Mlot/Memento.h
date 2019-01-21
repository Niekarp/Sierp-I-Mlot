#pragma once

namespace xo
{
	class Memento
	{
		friend class MementoFileSerializer;
	protected:
		virtual void save(std::ostream &) = 0;
		virtual void load(std::istream &) = 0;
	};
}