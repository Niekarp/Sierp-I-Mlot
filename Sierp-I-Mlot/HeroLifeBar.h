#pragma once
#include "IConsolePlane.h"

namespace xo
{
	class HeroLifeBar :
		public IConsolePlane
	{
	public:
		HeroLifeBar();

		// Inherited via IConsolePlane
		virtual Position position() override;
		virtual Position size() override;
		virtual PlaneType type() override;
		virtual void draw(const std::shared_ptr<Console::Buffer>&) override;

		void size(Position);
		void position(Position);
		void frame(char chr, WORD color);
		void progress(float);

	private:
		Position _pos;
		Position _size;
		char _frame_chr;
		WORD _frame_color;
		float _progress;
	};
}