#pragma once
#include "XOIGameMapHero.h"

namespace xo
{
	class ChartReader
	{
	public:
		ChartReader(const std::string &filename);
		void load(const std::string &filename);
		void put_notes_on(const std::shared_ptr<XOIGameMapHero> &);

	private:
		std::vector<XOIGameMapHero::Note> _notes;
	};
}