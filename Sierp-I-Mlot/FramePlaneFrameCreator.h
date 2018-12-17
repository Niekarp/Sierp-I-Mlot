#pragma once
#include "IFramedPlane.h"

struct FramePlaneFrameCreator
{
	FramePlaneFrameCreator(const std::shared_ptr<xo::IFramedPlane> &);
	void make_single_line_frame();
	void make_double_line_frame();

private:
	std::shared_ptr<xo::IFramedPlane> _frame_plane;
};

