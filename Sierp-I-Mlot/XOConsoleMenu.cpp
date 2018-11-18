#include "pch.h"
#include "XOConsoleMenu.h"
#include "CenteredFramedPlane.h"
#include "FileImagePlane.h"
#include "ButtonPlane.h"
#include "FramePlaneFrameCreator.h"

xo::XOConsoleMenu::XOConsoleMenu() :
	_main(true)
{
}

void xo::XOConsoleMenu::main(bool main)
{
	_main = main;
}

void xo::XOConsoleMenu::draw_on(const std::shared_ptr<Console>& output)
{
	auto menu_plane = std::make_shared<CenteredFramedPlane>();
	menu_plane->size({ 60, 40 });
	menu_plane->frame_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
	menu_plane->fill_color(BACKGROUND_BLUE | BACKGROUND_INTENSITY);

	float button_index;

	output->add_plane(menu_plane);

	if (_main)
	{
		menu_plane->size({ 60, 40 });

		auto logo_plane = std::make_shared<FileImagePlane>("resources/logo_pattern.txt");
		logo_plane->color(FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
		output->add_plane(logo_plane);

		logo_plane->position({
			output->width() / 2 - logo_plane->size().x / 2,
			output->height() / 2 - 14 });
		output->window_resize_event([logo_plane](auto new_width, auto new_height)
		{
			logo_plane->position({
				new_width / 2 - logo_plane->size().x / 2,
				new_height / 2 - 14 });
		});

		button_index = 0;
	} 
	else
	{
		menu_plane->size({ 60, (int)((6 + 1) * elements().size()) + 3 });
		
		button_index = -(elements().size() / 2.f - 0.5f);
	}	

	for (auto &element : elements())
	{
		if (element.second.size() == 0)
		{
			continue;
		}

		auto button_plane = std::make_shared<ButtonPlane>(button_index++);
		FramePlaneFrameCreator(button_plane).make_double_line_frame();
		button_plane->size({ 50, 6 });
		button_plane->frame_color(FOREGROUND_GREEN | FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
		button_plane->fill_color(BACKGROUND_BLUE | BACKGROUND_INTENSITY);
		button_plane->on_click([this, element] { element_call(element.first); });
		output->add_clickable_plane(button_plane);

		auto text = std::make_shared<TextConsolePlane>();
		text->font_size({ xo::conf::FONT_SMALL_SIZE_X, xo::conf::FONT_SMALL_SIZE_Y });
		text->text(element_text(element.first));
		text->load_font(xo::conf::FONT_SMALL_DIRECTORY);
		text->foreground(' ', BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
		text->background(' ', BACKGROUND_BLUE | BACKGROUND_INTENSITY);
		text->position({ 0, 0 });
		button_plane->text(text);
	}
}

void xo::XOConsoleMenu::clear(const std::shared_ptr<Console>& output)
{
	output->clear_planes();
}

