#pragma once
#include "IXOVIew.h"

class IXOGameMap : public IXOView
{
public:
	enum class Figure
	{
		None,
		X,
		O
	};

	virtual size_t width() = 0;
	virtual void width(size_t) = 0;
	virtual size_t height() = 0;
	virtual void height(size_t) = 0;
	virtual void put(Figure figure, size_t x, size_t y) = 0;
};