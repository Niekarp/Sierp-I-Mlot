#include "pch.h"
#include "XOViewElementRegistrar.h"

using namespace xo;

xo::XOViewElementRegistrar::XOViewElementRegistrar() :
	_call_thread(0)
{
	_call_mutex = CreateMutex(NULL, FALSE, NULL);
}

void xo::XOViewElementRegistrar::register_element(const XOViewElement &element)
{
	_elements[element.id].push_back(element);
}

std::string xo::XOViewElementRegistrar::element_text(const std::string & id)
{
	if (_elements[id].size() == 0) 
	{
		return "";
	}
	return _elements[id].front().text;
}

void xo::XOViewElementRegistrar::element_call(const std::string & id)
{
	CHKERR_DWORD(WaitForSingleObject(_call_mutex, INFINITE));
	OutputDebugStringA("XOViewElementRegistrar: after WaitForSingleObject\n");
	
 	/*auto current_thread = GetCurrentThreadId();
	if (_call_thread != NULL && current_thread != _call_thread)
	{
		return;
	}
	_call_thread = current_thread;*/
	
	for (auto element : _elements[id])
	{
		if (element.callback)
		{
			element.callback();
		}
	}

	CHKERR_MUTEX(ReleaseMutex(_call_mutex));
}

std::map<std::string, std::vector<XOViewElement>>& xo::XOViewElementRegistrar::elements()
{
	return _elements;
}

size_t xo::XOViewElementRegistrar::elements_size()
{
	size_t size = 0;
	
	for (auto element : _elements)
	{
		size += element.second.size();
	}

	return size;
}
