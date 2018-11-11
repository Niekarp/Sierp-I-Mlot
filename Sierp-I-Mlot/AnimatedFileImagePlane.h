#pragma once
#include "IConsolePlane.h"
#include "FileImagePlane.h"

struct AnimatedFileImagePlane : 
	public IConsolePlane
{
	AnimatedFileImagePlane();

	Position position() override;
	Position size() override;
	PlaneType type() override;
	void draw(const std::shared_ptr<Console::Buffer> &) override;

	void color(WORD color);
	void load_frame(const char *filename);
	void position(Position position);
	void next();
	void frame(size_t);
	unsigned frame_index();
	unsigned last_frame_index();

private:
	std::vector<std::shared_ptr<FileImagePlane>> _image_frames;
	WORD _color;
	Position _position;
	Position _max_frame_sz;
	unsigned _current_frame_index;
};

