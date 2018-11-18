#pragma once
#include "XOIView.h"

namespace xo
{
	class XOIMessage : 
		public virtual XOIView
	{
	public:
		virtual void text(const std::string &msg) = 0;
		virtual void time(size_t ms) = 0;
	};
}