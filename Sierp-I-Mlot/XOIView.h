#pragma once
#include "XOViewElement.h"

namespace xo
{
	class IXOView
	{
	public:
		virtual void register_element(const XOViewElement&) = 0;
	};
}
