#pragma once
#include "XOIView.h"
#include "XOGameLogic.h"

namespace xo
{
	class XOIGameMapXO :
		public virtual XOIView
	{
	public:
		virtual size_t width() = 0;
		virtual void width(size_t) = 0;
		virtual size_t height() = 0;
		virtual void height(size_t) = 0;
		virtual void put(PlayerSymbol symbol, size_t x, size_t y) = 0;
		virtual void select(PlayerSymbol symbol, size_t x, size_t y) = 0;
		virtual void clear_fields() = 0;
		virtual void clear_selection() = 0;
		virtual void player_symbol(PlayerSymbol) = 0;
	};
}
