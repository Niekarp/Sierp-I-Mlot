#pragma once
#include "CenteredFramedPlane.h"

struct FramePlaneFrameCreator
{
	FramePlaneFrameCreator(const std::shared_ptr<CenteredFramedPlane> &);
	void make_single_line_frame();
	void make_double_line_frame();

private:
	std::shared_ptr<CenteredFramedPlane> _frame_plane;
};

