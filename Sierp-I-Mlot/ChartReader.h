#pragma once
#include "XOIGameMapHero.h"
#include "NoteContainer.h"

namespace xo
{
	class ChartReader
	{
	public:
		ChartReader();
		void load(const std::string &filename);
		std::shared_ptr<NoteContainer> notes();

	private:
		std::shared_ptr<NoteContainer> _notes;
	};
}