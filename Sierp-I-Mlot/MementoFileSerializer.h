#pragma once
#include "Memento.h"

namespace xo
{
	class MementoFileSerializer
	{
	public:
		MementoFileSerializer(const std::shared_ptr<Memento> &);
		void save(const std::string &path);
		void load(const std::string &path);

	private:
		std::shared_ptr<Memento> _memento;
	};
}