#pragma once
#include "XOIGameMapHero.h"

namespace xo
{
	class ChartReader
	{
	public:
		ChartReader(const char *filename);
		void load(const char *filename);
		void put_notes_on(const std::shared_ptr<XOIGameMapHero> &);

	private:
		std::vector<XOIGameMapHero::Note> _notes;
	};
}