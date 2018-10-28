#include "stdafx.h"
#include "ButtonPlane.h"

ButtonPlane::ButtonPlane(int position) :
	CenteredFramedPlane(position)
{
}

void ButtonPlane::draw(const std::shared_ptr<Console::Buffer>& buffer)
{
	CenteredFramedPlane::draw(buffer);
	if (_text)
	{
		auto text_absolute_pos = _text->position();
		auto button_absolute_pos = position();
		auto text_sz = _text->size();
		Position relative_pos =
		{
			button_absolute_pos.x + text_absolute_pos.x - text_sz.x / 2,
			button_absolute_pos.y + text_absolute_pos.y - text_sz.y / 2
		};
		
		_text->position(relative_pos);
		_text->draw(buffer);
		_text->position(text_absolute_pos);
	}
}

void ButtonPlane::click(IConsolePlane::Position position, DWORD btn, DWORD flag)
{
	if (_click_callback)
	{
		_click_callback();
	}
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
}

void ButtonPlane::text(std::shared_ptr<TextConsolePlane> text)
{
	_text = text;
}

void ButtonPlane::on_click(std::function<void()> callback)
{
	_click_callback = callback;
}
