#pragma once
#include "XOIMenu.h"
#include "XOViewElement.h"
#include "Console.h"

namespace xo
{
	class XOConsoleMenu :
		public XOIMenu
	{
	public:
		void register_element(const XOViewElement &) override;

		void draw_on(const std::shared_ptr<Console> &output);
		void clear(const std::shared_ptr<Console> &output);

	private:
		/*struct _Element
		{
			const char *id;
			const char *text;
			std::function<void()> callback;
		};*/
		// std::vector<XOViewElement> _elements;
	};
}