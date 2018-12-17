#pragma once
#include "XOApplication.h"

namespace xo
{
	class XOConsoleApplication :
		public XOApplication
	{
	public:
		std::shared_ptr<XOIOutput> create_output();
	};
}