#pragma once
#include "XOIView.h"
#include "NoteContainer.h"
#include "PlayerSymbol.h"

namespace xo
{
	class XOIGameMapHero :
		public XOIView
	{
	public:
		virtual void start(long long start, long long omitt, long long duration) = 0;
		virtual void button(int string_n, bool pressed) = 0;
		virtual void progress(float) = 0;
		virtual void life(float) = 0;
		virtual void reset() = 0;
		virtual void player_symbol(xo::PlayerSymbol) = 0;
		virtual void feed(const std::shared_ptr< NoteContainer> &notes) = 0;
	};
}

