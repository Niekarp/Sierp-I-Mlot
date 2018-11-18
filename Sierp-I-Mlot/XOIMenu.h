#pragma once
#include "XOIView.h"

namespace xo
{
	class XOIMenu : 
		public virtual XOIView
	{
	public:
		virtual void main(bool) = 0;
	};
}
