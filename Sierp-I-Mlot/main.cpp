#include "pch.h"
#include "XOApplication.h"

int main()
{
	auto xo_application = xo::XOApplication::get_instance();
	xo_application->run();

	return 0;
}
