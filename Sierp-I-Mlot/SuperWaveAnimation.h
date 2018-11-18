#pragma once
#include "IAnimation.h"

struct SuperWaveAnimation : public IAnimation
{
	SuperWaveAnimation(int start_frame, int n_rows);
	void draw(const std::shared_ptr<Console::Buffer> &buffer, size_t frame) override;
	bool end() override;
	bool continue_() override;

	void speed(int);
	void center(int x, int y);

private:
	int _start_frame;
	int _n_rows;
	int _speed;
	int _center_x;
	int _center_y;
};
