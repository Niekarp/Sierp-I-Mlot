#pragma once
#include "IConsolePlane.h"

namespace xo
{
	class IFramedPlane
	{
	public:
		virtual void fill_char(char fill_char) = 0;
		virtual void frame_chars(char left_chr, char top_chr, char right_chr, char bottom_chr) = 0;
		virtual void corner_chars(char left_top_chr, char right_top_chr, char left_bottom_chr, char right_bottom_chr) = 0;
	};
}