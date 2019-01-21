#pragma once
#include "XOApplication.h"
#include "XOIView.h"

namespace xo
{
	class XOConsoleApplication :
		public XOApplication
	{
	protected:
		virtual std::shared_ptr<XOIOutput> create_output() override;
	};
}