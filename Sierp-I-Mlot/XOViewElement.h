#pragma once

namespace xo
{
	class XOViewElement
	{
	public:
		const std::string id;
		const std::string text;
		std::function<void()> callback;
	};
}