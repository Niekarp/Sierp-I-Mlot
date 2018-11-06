#pragma once
#include "IClickableConsolePlane.h"
#include "CenteredFramedPlane.h"

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
	void foreground(WORD color);
	void background(WORD color);
	void put(int col, int row, int f);

private:
	int _cols;
	int _rows;
	Position _size;
	Position _position;
	WORD _color_foreground;
	WORD _color_background;
	std::vector<int> _fields;
	std::shared_ptr<CenteredFramedPlane> _x_plane;
};