#include "pch.h"
#include "ButtonPlane.h"

ButtonPlane::ButtonPlane(float position) :
	CenteredFramedPlane(position)
{
}

void ButtonPlane::draw(const std::shared_ptr<Console::Buffer>& buffer)
{
	CenteredFramedPlane::draw(buffer);
	if (_text)
	{
		auto button_absolute_pos = position();
		auto text_sz = _text->size();
		Position relative_pos =
		{
			button_absolute_pos.x - text_sz.x / 2,
			button_absolute_pos.y - text_sz.y / 2
		};
		
		_text->position(relative_pos);
		_text->draw(buffer);
	}
}

void ButtonPlane::click(IConsolePlane::Position position, DWORD btn, DWORD flag)
{
	CenteredFramedPlane::fill_color(BACKGROUND_BLUE);
	if (_text)
	{
		_text->background(' ', BACKGROUND_BLUE);
	}
}

void ButtonPlane::click_release()
{
	CenteredFramedPlane::fill_color(BACKGROUND_BLUE | BACKGROUND_INTENSITY);
	if (_text)
	{
		_text->background(' ', BACKGROUND_BLUE | BACKGROUND_INTENSITY);
	}

	if (_click_callback)
	{
		_click_callback();
	}
}

void ButtonPlane::text(std::shared_ptr<TextConsolePlane> text)
{
	_text = text;
}

void ButtonPlane::on_click(std::function<void()> callback)
{
	_click_callback = callback;
}
