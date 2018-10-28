#pragma once
#include "IClickableConsolePlane.h"
#include "CenteredFramedPlane.h"
#include "TextConsolePlane.h"

struct ButtonPlane :
	public IClickableConsolePlane,
	public CenteredFramedPlane
{
	ButtonPlane(int position = 0);
	void draw(const std::shared_ptr<Console::Buffer> &) override;
	void click(IConsolePlane::Position position, DWORD btn, DWORD flag)  override;
	void click_release() override;
	
	void text(std::shared_ptr<TextConsolePlane> text);
	void on_click(std::function<void()> callback);

private:
	std::shared_ptr<TextConsolePlane> _text;
	std::function<void()> _click_callback;
};

