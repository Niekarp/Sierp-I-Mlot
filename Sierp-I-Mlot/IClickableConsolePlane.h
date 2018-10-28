#pragma once
#include "IConsolePlane.h"

struct IClickableConsolePlane : 
	public virtual IConsolePlane
{
	virtual void click(IConsolePlane::Position position, DWORD btn, DWORD flag) = 0;
	virtual void click_release() = 0;
};

