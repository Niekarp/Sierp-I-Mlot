#include "stdafx.h"
#include "AnimatedFileImagePlane.h"

AnimatedFileImagePlane::AnimatedFileImagePlane() :
	_color(0),
	_position({0, 0}),
	_current_frame_index(0)
{
}

AnimatedFileImagePlane::Position AnimatedFileImagePlane::position()
{
	return _position;
}

AnimatedFileImagePlane::Position AnimatedFileImagePlane::size()
{
	return _max_frame_sz;
}

AnimatedFileImagePlane::PlaneType AnimatedFileImagePlane::type()
{
	return PlaneType::CENTERED;
}

void AnimatedFileImagePlane::draw(const std::shared_ptr<Console::Buffer>& buffer)
{
	auto current_frame = _image_frames[_current_frame_index % _image_frames.size()];
	current_frame->draw(buffer);
}

void AnimatedFileImagePlane::color(WORD color)
{
	_color = color;
	for (auto &frame : _image_frames)
	{
		frame->color(color);
	}
}

void AnimatedFileImagePlane::load_frame(const char * filename)
{
	auto frame = std::make_shared<FileImagePlane>(filename);
	frame->color(_color);
	frame->position({ -1000, -1000 });
	_image_frames.push_back(frame);
}

void AnimatedFileImagePlane::position(Position position)
{
	_max_frame_sz = {};

	for (auto &frame : _image_frames)
	{
		auto frame_sz = frame->size();
		if (frame_sz.x * frame_sz.y > _max_frame_sz.x * _max_frame_sz.y)
		{
			_max_frame_sz = frame_sz;
		}
	}

	auto cx = position.x + _max_frame_sz.x / 2;
	auto cy = position.y + _max_frame_sz.y / 2;

	for (auto &frame : _image_frames)
	{
		auto frame_sz = frame->size();
		
		frame->position(
		{
			cx - frame_sz.x / 2,
			cy - frame_sz.y / 2
		});
	}
}

void AnimatedFileImagePlane::next()
{
	++_current_frame_index;
}

unsigned AnimatedFileImagePlane::frame_index()
{
	return _current_frame_index;
}

unsigned AnimatedFileImagePlane::end_frame_index()
{
	return _image_frames.size() - 1;
}
