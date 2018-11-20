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
		void _draw_particles(const std::shared_ptr<Console::Buffer> &buffer, float pbx,
			float pby, float pex, float pey, float p1cx, float p1cy);
		Position _position;
		Position _size;
		char _frame_chr;
		WORD _frame_color;
		float _progress;
		long long _frame;
	};
}