#pragma once
#include "IConsolePlane.h"

struct Menu
{
	struct Option
	{
		const char *name;
		std::function<void()> callback;
	};

	Menu();
	void add_option(const Option &);
	void draw_on(const std::shared_ptr<Console> &output);

private:
	std::vector<Option> _options;
};