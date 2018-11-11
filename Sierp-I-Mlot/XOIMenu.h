#pragma once
#include "XOIView.h"

namespace xo
{
	class XOIMenu : public XOIView
	{
	public:
		virtual void main(bool) = 0;
	};
}
