#pragma once
#include "XOIGameMapXO.h"
#include "Console.h"
#include "XOConsoleMapPlane.h"

namespace xo
{
	class XOConsoleGameMapXO : public XOIGameMapXO
	{
	public:
		XOConsoleGameMapXO();

		void register_element(const XOViewElement &) override;		
		size_t width() override;
		void width(size_t) override;
		size_t height() override;
		void height(size_t) override;
		void put(PlayerSymbol symbol, size_t x, size_t y) override;
		void clear_fields() override;

		void draw_on(const std::shared_ptr<Console> &output);
	
	private:
		void _on_click(int col, int row);
		size_t _width;
		size_t _height;
		std::shared_ptr<XOConsoleMapPlane> _map_plane;
		size_t _click_row;
		size_t _click_col;
		std::function<void(int)> _key_down_callback;
	};
}
