#include "pch.h"
#include "XOTimer.h"

xo::XOTimer::XOTimer(const std::function<void()> &callback) :
	_callback(callback)
{
}

xo::XOTimer::~XOTimer()
{
}

void xo::XOTimer::after(long long delay)
{
	struct Tmp
	{
		HANDLE queue_timer;
		std::function<void()> callback;
	};
	auto tmp = new Tmp{ NULL, _callback };

	auto result = CreateTimerQueueTimer(&tmp->queue_timer, NULL, [](LPVOID tmp_, auto _tow) {
		auto tmp = (Tmp *)tmp_;
		tmp->callback();

		CHKERR_BOOL(DeleteTimerQueueTimer(NULL, tmp->queue_timer, NULL));
		delete tmp;
	}, tmp, delay, 0, 0);
	CHKERR_BOOL(result);
}
