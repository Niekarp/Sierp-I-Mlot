#pragma once
#include "IAnimation.h"

struct SuperWaveAnimation : public IAnimation
{
	SuperWaveAnimation(int n_rows);
	void draw(const std::shared_ptr<Console::Buffer> &buffer, size_t frame) override;
	bool end() override;
	bool continue_() override;

private:
	int _n_rows;
};
