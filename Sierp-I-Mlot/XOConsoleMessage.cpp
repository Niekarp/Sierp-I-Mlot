#include "pch.h"
#include "XOConsoleMessage.h"
#include "AnimationChain.h"

using namespace xo;

xo::XOConsoleMessage::XOConsoleMessage() :
	_msg("empty"),
	_time_ms(0),
	_text_plane(std::make_shared<TextConsolePlane>()),
	_end(false)
{
}

void XOConsoleMessage::text(const std::string & msg)
{
	_msg = msg;
	
	_text_plane = std::make_shared<TextConsolePlane>();
	_text_plane->load_font(xo::conf::FONT_NORMAL_DIRECTORY);
	_text_plane->text(_msg);
	_text_plane->foreground(0, FOREGROUND_RED | FOREGROUND_BLUE 
		| FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}

void xo::XOConsoleMessage::draw(const std::shared_ptr<Console::Buffer>& buffer, size_t frame)
{
	auto bx = 0;
	auto by = 0;
	auto ex = buffer->screen_width();
	auto ey = buffer->screen_height();

	for (auto ix = 0; ix < ex; ++ix)
	{
		for (auto iy = 0; iy < ey; ++iy)
		{
			buffer->put(ix, iy, ' ');
		}
	}

	_text_plane->position({ (ex - bx) / 2 - _text_plane->size().x / 2,
		(ey - by) / 2 - _text_plane->size().y / 2 });
	_text_plane->draw(buffer);

	if (frame * xo::conf::ANIMATION_FRAME_FREQUENCY >= _time_ms)
	{
		_end = true;
		element_call("after");
	}
}

bool xo::XOConsoleMessage::end()
{
	return _end;
}

bool xo::XOConsoleMessage::continue_()
{
	return false;
}

void xo::XOConsoleMessage::draw_on(const std::shared_ptr<Console>& console)
{
	console->clear_planes();
	auto animation_chain = std::make_shared<AnimationChain>();
	animation_chain->add(shared_from_this());
	animation_chain->on_end([this](auto frame) {
		element_call("after");
	});

	console->animate_async(animation_chain, xo::conf::ANIMATION_FRAME_FREQUENCY);
}

void xo::XOConsoleMessage::time(size_t ms)
{
	_time_ms = ms;
}
