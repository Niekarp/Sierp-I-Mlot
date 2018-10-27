#pragma once

namespace xo
{
	enum class XOViewTag
	{
		game,
		main_menu
	};

	class XOViewManager
	{
	public:
		XOViewTag get_current_view();
	private:
		XOViewTag _current_view;
	};
}
