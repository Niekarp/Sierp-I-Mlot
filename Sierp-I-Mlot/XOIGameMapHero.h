#pragma once
#include "XOIView.h"

namespace xo
{
	class XOIGameMapHero :
		public XOIView
	{
	public:
		struct Note
		{
			unsigned time;
			unsigned tone;
			unsigned duration;
		};

		virtual void put(const Note &) = 0;
		virtual void start(long long wait) = 0;
		virtual void button(int string_n, bool pressed) = 0;
	};
}

