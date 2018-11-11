#pragma once
#include "IAnimation.h"

class FullScreenTextAnimation :
	public IAnimation
{
public:
	FullScreenTextAnimation(const char *line1, const char *line2);

	void draw(const std::shared_ptr<Console::Buffer>& buffer, size_t frame) override;
	bool end() override;
	bool continue_() override;

	void foreground(char chr, WORD color);
	void background(char chr, WORD color);

private:
	const char *_line1;
	const char *_line2;
	bool _end;
	char _foreground_chr;
	char _background_chr;
	WORD _foreground_color;
	WORD _background_color;
};