#pragma once
#include "XOIView.h"

namespace xo
{
	class XOViewElementRegistrar :
		public virtual XOIView
	{
	public:
		XOViewElementRegistrar();

		void register_element(const XOViewElement&) override;

	protected:
		std::string element_text(const std::string &id);
		void element_call(const std::string &id);
		std::map<std::string, std::vector<XOViewElement>> &elements();
		size_t elements_size();

	private:
		std::map<std::string, std::vector<XOViewElement>> _elements;
		HANDLE _call_mutex;
		DWORD _call_thread;
	};
}