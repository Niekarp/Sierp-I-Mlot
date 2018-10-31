#pragma once
#include "IXOMenu.h"
#include "Console.h"
#include "XOViewElement.h"


namespace xo
{
	class XOIConsoleMenu :
		public IXOMenu
	{
	public:
		void register_element(const XOViewElement&) override;

		void draw_on(const std::shared_ptr<Console> &output);

	private:
		std::vector<XOViewElement> _elements;
	};
}