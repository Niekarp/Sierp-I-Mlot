#pragma once

namespace xo
{
	class XOViewElement
	{
	public:
		const char *id;
		const char *text;
		std::function<void()> callback;
	};
}