#pragma once
#include "IAnimation.h"
#include "XOGameLogic.h"

namespace xo
{
	class GameHeroAnimation :
		public IAnimation
	{
	public:
		GameHeroAnimation();

		// Inherited via IAnimation
		virtual void draw(const std::shared_ptr<Console::Buffer>& buffer, size_t frame) override;
		virtual bool end() override;
		virtual bool continue_() override;

		void speed(int);
		void start_frame(int);
		void center(int x, int y);
		void player_symbol(xo::PlayerSymbol symbol);

	private:
		int _speed;
		int _start_frame;
		int _center_x;
		int _center_y;
		xo::PlayerSymbol _player_symbol;
	};
}