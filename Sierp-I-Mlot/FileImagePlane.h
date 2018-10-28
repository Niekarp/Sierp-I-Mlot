#pragma once
#include "IConsolePlane.h"

struct FileImagePlane : public IConsolePlane
{
	FileImagePlane(const char *filename);
	Position position() override;
	Position size() override;
	PlaneType type() override;
	void draw(const std::shared_ptr<Console::Buffer> &) override;

	void position(Position);
	void load(const char *filename);
	void color(WORD color);

private:
	std::vector<char> _pattern;
	Position _pattern_pos;
	Position _pattern_size;
	WORD _color;
};

