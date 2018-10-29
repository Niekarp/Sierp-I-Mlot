#pragma once
#include "IXOMenu.h"
#include "Console.h"

namespace xo
{
	class XOIConsoleMenu :
		public IXOMenu
	{
	public:
		void register_element(const XOViewElement &) override;

		void draw_on(const std::shared_ptr<Console> &output);

	private:
		struct _Element
		{
			const char *id;
			const char *text;
			std::function<void()> callback;
		};
		std::vector<_Element> _elements;
	};
}