#pragma once
#include "XOIOutput.h"

namespace xo
{
	class XOConsoleOutput : public XOIOutput
	{
	public:
		XOConsoleOutput(XOActionLogic &);

		void keep_drawing() override;
		void change_drawing_view(XOViewTag view) override;
		void draw_symbol(unsigned x, unsigned y) override;
	};
}
