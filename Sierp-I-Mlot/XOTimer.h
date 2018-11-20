#pragma once

namespace xo
{
	class XOTimer
	{
	public:
		XOTimer(const std::function<void()> &);
		~XOTimer();
		void after(long long delay);

	private:
		std::function<void()> _callback;
	};
}