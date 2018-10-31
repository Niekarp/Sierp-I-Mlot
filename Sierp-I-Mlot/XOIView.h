#pragma once
#include "XOViewElement.h"

namespace xo
{
	class XOIView
	{
	public:
		virtual void register_element(const XOViewElement&) = 0;
	};
}