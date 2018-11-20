#pragma once
#include "IConsolePlane.h"
#include "IClickableConsolePlane.h"


	class FileImagePlane :
		public IClickableConsolePlane
	{
	public:
		enum class Gravity
		{
			DEFAULT,
			LEFTDOWN
		};
		FileImagePlane(const std::string &filename);
		Position position() override;
		Position size() override;
		PlaneType type() override;
		void draw(const std::shared_ptr<Console::Buffer> &) override;

		void click(IConsolePlane::Position position, DWORD btn, DWORD flag) override;
		void click_release() override;

		void position(Position);
		void load(const std::string &filename);
		void color(WORD color);
		void gravity(Gravity g);

		void click_event(const std::function<void()> &);

	private:
		std::vector<char> _pattern;
		Position _pattern_pos;
		Position _real_pattern_pos;
		Position _pattern_size;
		Position _real_pattern_size;
		WORD _color;
		Gravity _gravity;
		std::function<void()> _click_callback;
		bool _clicked;
	};
