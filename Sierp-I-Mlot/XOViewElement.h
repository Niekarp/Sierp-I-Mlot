#pragma once

namespace xo
{
	class XOViewElement
	{
	public:
		std::string id;
		std::string text;
		std::function<void()> callback;
	};
}