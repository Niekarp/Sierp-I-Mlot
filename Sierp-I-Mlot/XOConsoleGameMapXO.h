#pragma once
#include "XOIGameMapXO.h"
#include "Console.h"
#include "XOConsoleMapPlane.h"
#include "XOViewElementRegistrar.h"
#include "XOGameLogic.h"
#include "FileImagePlane.h"

namespace xo
{
	class XOConsoleGameMapXO : 
		public XOIGameMapXO,
		public XOViewElementRegistrar
	{
	public:
		XOConsoleGameMapXO();	

		// Inherited via XOIGameMapXO
		size_t width() override;
		void width(size_t) override;
		size_t height() override;
		void height(size_t) override;
		void put(PlayerSymbol symbol, size_t x, size_t y) override;
		void clear_fields() override;
		void select(PlayerSymbol symbol, size_t x, size_t y) override;
		void clear_selection() override;
		void player_symbol(PlayerSymbol) override;

		void draw_on(const std::shared_ptr<Console> &output);
		void field_x(const std::string &path);
		void field_o(const std::string &path);
	
	private:
		void _on_click(int col, int row);

		std::string _field_x_path;
		std::string _field_o_path;
		size_t _width;
		size_t _height;
		std::shared_ptr<XOConsoleMapPlane> _map_plane;
		std::shared_ptr<FileImagePlane> _back_button;
		size_t _click_row;
		size_t _click_col;
		std::function<void(int)> _key_down_callback;
		PlayerSymbol _player_symbol;

		// Inherited via XOIGameMapXO
	};
}
