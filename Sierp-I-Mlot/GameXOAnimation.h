#pragma once
#include "IAnimation.h"
#include "XOGameLogic.h"

namespace xo
{
	class GameXOAnimation :
		public IAnimation
	{
	public:
		GameXOAnimation(int start_frame);

		// Inherited via IAnimation
		virtual void draw(const std::shared_ptr<Console::Buffer>& buffer, size_t frame) override;
		virtual bool end() override;
		virtual bool continue_() override;

		void speed(int);
		void player_symbol(xo::PlayerSymbol);

	private:
		int _speed;
		int _start_frame;
		xo::PlayerSymbol _player_symbol;
	};
}