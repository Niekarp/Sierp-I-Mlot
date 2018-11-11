#include "pch.h"
#include "FullScreenTextAnimation.h"

FullScreenTextAnimation::FullScreenTextAnimation(const char *line1, const char *line2) :
	_line1(line1),
	_line2(line2),
	_end(false),
	_foreground_chr(' '),
	_background_chr(' '),
	_foreground_color(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY),
	_background_color(0)
{
}

void FullScreenTextAnimation::draw(const std::shared_ptr<Console::Buffer>& buffer,
	size_t frame)
{
}

bool FullScreenTextAnimation::end()
{
	return _end;
}

bool FullScreenTextAnimation::continue_()
{
	return false;
}

void FullScreenTextAnimation::foreground(char chr, WORD color)
{
	_foreground_chr = chr;
	_foreground_color = color;
}

void FullScreenTextAnimation::background(char chr, WORD color)
{
	_background_chr = chr;
	_background_color = color;
}
