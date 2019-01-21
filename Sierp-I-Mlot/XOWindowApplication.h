#pragma once
#include "XOApplication.h"

namespace xo
{
	class XOWindowApplication :
		public XOApplication
	{
	protected:
		std::shared_ptr<XOIOutput> create_output() override;
	};
}