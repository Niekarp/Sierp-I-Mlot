#pragma once
#include "XOIGameMap.h"
#include "Console.h"

namespace xo
{
	class XOConsoleGameMap : public XOIGameMap
	{
	public:
		void register_element(const XOViewElement &) override;

		void draw_on(const std::shared_ptr<Console> &output);

		size_t width() override;
		void width(size_t) override;
		size_t height() override;
		void height(size_t) override;
		void put(PlayerSymbol symbol, size_t x, size_t y) override;
	private:
		size_t _width;
		size_t _height;
	};
}
