#pragma once
#include "IConsolePlane.h"

namespace xo
{
	class HeroProgressBarPlane :
		public IConsolePlane
	{
	public:
		HeroProgressBarPlane();

		// Inherited via IConsolePlane
		virtual Position position() override;
		virtual Position size() override;
		virtual PlaneType type() override;
		virtual void draw(const std::shared_ptr<Console::Buffer>&) override;

		void frame(char chr, WORD color);
		void position(Position);
		void size(Position);
		void progress(float);

	private:
		Position _position;
		Position _size;
		char _frame_chr;
		WORD _frame_color;
		float _progress;
		long long _frame;
	};
}