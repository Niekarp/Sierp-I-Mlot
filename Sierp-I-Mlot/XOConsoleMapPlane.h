#pragma once
#include "IClickableConsolePlane.h"

class XOConsoleMapPlane :
	public virtual IClickableConsolePlane
{
public:
	XOConsoleMapPlane(int cols = 3, int rows = 3);

	Position position() override;
	Position size() override;
	PlaneType type() override;
	void draw(const std::shared_ptr<Console::Buffer> &) override;
	void click(IConsolePlane::Position position, DWORD btn, DWORD flag) override;
	void click_release() override;

	void size(const Position &);
	void color(WORD color);

private:
	int _cols;
	int _rows;
	Position _size;
	Position _position;
	WORD _color;
};