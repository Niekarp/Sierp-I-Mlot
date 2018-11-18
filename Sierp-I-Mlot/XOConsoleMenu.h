#pragma once
#include "XOIMenu.h"
#include "XOViewElement.h"
#include "Console.h"
#include "XOViewElementRegistrar.h"

namespace xo
{
	class XOConsoleMenu :
		public XOIMenu,
		public XOViewElementRegistrar
	{
	public:
		XOConsoleMenu();

		void main(bool) override;

		void draw_on(const std::shared_ptr<Console> &output);
		void clear(const std::shared_ptr<Console> &output);

	private:
		bool _main;
		/*struct _Element
		{
			const std::string &id;
			const std::string &text;
			std::function<void()> callback;
		};*/
		// std::vector<XOViewElement> _elements;
	};
}