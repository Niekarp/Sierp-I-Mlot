#pragma once

namespace xo
{
	class XOIDelegateExecutor
	{
	public:
		virtual void execute(const std::function<void()> &) = 0;
	};
}