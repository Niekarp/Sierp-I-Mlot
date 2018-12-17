#include "pch.h"
#include "FramePlaneFrameCreator.h"


FramePlaneFrameCreator::FramePlaneFrameCreator(
	const std::shared_ptr<xo::IFramedPlane>& frame_plane) :
	_frame_plane(frame_plane)
{
}

void FramePlaneFrameCreator::make_single_line_frame()
{
	_frame_plane->fill_char(' ');
	_frame_plane->frame_chars('\xb3', '\xc4', '\xb3', '\xc4');
	_frame_plane->corner_chars('\xda', '\xbf', '\xc0', '\xd9');
}

void FramePlaneFrameCreator::make_double_line_frame()
{
	_frame_plane->fill_char(' ');
	_frame_plane->frame_chars('\xba', '\xcd', '\xba', '\xcd');
	_frame_plane->corner_chars('\xc9', '\xbb', '\xc8', '\xbc');
}
