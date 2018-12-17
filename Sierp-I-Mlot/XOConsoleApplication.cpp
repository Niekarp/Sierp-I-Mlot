#include "pch.h"
#include "XOConsoleApplication.h"
#include "XOConsoleOutput.h"

using namespace xo;

std::shared_ptr<XOIOutput> xo::XOConsoleApplication::create_output()
{
	return std::make_shared<XOConsoleOutput>();
}
