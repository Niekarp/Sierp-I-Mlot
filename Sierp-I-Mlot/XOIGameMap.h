#pragma once
#include "XOIView.h"
#include "XOGameLogic.h"

namespace xo
{
	class XOIGameMap : public XOIView
	{
	public:
		virtual size_t width() = 0;
		virtual void width(size_t) = 0;
		virtual size_t height() = 0;
		virtual void height(size_t) = 0;
		virtual void put(PlayerSymbol symbol, size_t x, size_t y) = 0;
		virtual void clear_fields() = 0;
	};
}
