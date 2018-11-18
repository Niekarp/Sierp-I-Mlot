#pragma once
#include "IAnimation.h"

class FullScreenTextAnimation :
	public IAnimation
{
public:
	FullScreenTextAnimation(const std::string &line1, const std::string &line2);

	void draw(const std::shared_ptr<Console::Buffer>& buffer, size_t frame) override;
	bool end() override;
	bool continue_() override;

	void foreground(char chr, WORD color);
	void background(char chr, WORD color);

private:
	std::string _line1;
	std::string _line2;
	bool _end;
	char _foreground_chr;
	char _background_chr;
	WORD _foreground_color;
	WORD _background_color;
};