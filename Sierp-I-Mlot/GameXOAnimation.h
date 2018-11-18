#pragma once
#include "IAnimation.h"

namespace xo
{
	class GameXOAnimation :
		public IAnimation
	{
	public:
		GameXOAnimation(int start_frame, int turn);

		// Inherited via IAnimation
		virtual void draw(const std::shared_ptr<Console::Buffer>& buffer, size_t frame) override;
		virtual bool end() override;
		virtual bool continue_() override;

		void speed(int);

	private:
		int _speed;
		int _start_frame;
		int _turn;
	};
}