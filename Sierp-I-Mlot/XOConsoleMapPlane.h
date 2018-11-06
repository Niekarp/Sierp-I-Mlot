#pragma once
#include "IClickableConsolePlane.h"
#include "CenteredFramedPlane.h"
#include "FileImagePlane.h"

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
	void images(const std::vector<std::shared_ptr<FileImagePlane>> &);
	void click(std::function<void(int, int)> callback);

private:
	int _cols;
	int _rows;
	Position _size;
	Position _position;
	WORD _color_foreground;
	WORD _color_background;
	std::vector<int> _fields;
	std::shared_ptr<CenteredFramedPlane> _x_plane;
	std::vector<std::shared_ptr<FileImagePlane>> _images;
	std::function<void(int, int)> _callback_click;
};