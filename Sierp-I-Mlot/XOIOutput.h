#pragma once
#include "XOViewManager.h"
#include "XOActionLogic.h"
#include "XOIView.h"

namespace xo
{
	enum class XOViewTag
	{
		main_menu,
		game
	};

	class XOIOutput
	{
	public:
		XOIOutput(XOActionLogic &);
		virtual ~XOIOutput();

		virtual void keep_drawing() = 0;
		virtual void change_drawing_view(XOViewTag view) = 0;
		virtual void draw_symbol(unsigned x, unsigned y) = 0;
	private:
		XOActionLogic &_action_logic;
		XOViewTag _current_view;
		std::vector<XOIView> _views;
	};
}
