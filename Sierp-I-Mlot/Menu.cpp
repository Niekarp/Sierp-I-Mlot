#include "pch.h"
#include "Menu.h"

#include "CenteredFramedPlane.h"
#include "FileImagePlane.h"
#include "ButtonPlane.h"
#include "FramePlaneFrameCreator.h"

Menu::Menu()
{
}

void Menu::add_option(const Option& option)
{
	_options.push_back(option);
}

void Menu::draw_on(const std::shared_ptr<Console>& output)
{
	auto menu_plane = std::make_shared<CenteredFramedPlane>();
	menu_plane->size({ 60, 40 });
	menu_plane->frame_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
	menu_plane->fill_color(BACKGROUND_BLUE | BACKGROUND_INTENSITY);
	output->add_plane(menu_plane);

	auto logo_plane = std::make_shared<FileImagePlane>("resources/logo_pattern.txt");
	logo_plane->color(FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
	output->add_plane(logo_plane);

	int i = 0;
	for (auto &option : _options)
	{
		auto button_plane = std::make_shared<ButtonPlane>(i++);
		FramePlaneFrameCreator(button_plane).make_double_line_frame();
		button_plane->size({ 50, 6 });
		button_plane->frame_color(FOREGROUND_GREEN | FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
		button_plane->fill_color(BACKGROUND_BLUE | BACKGROUND_INTENSITY);
		button_plane->on_click(option.callback);
		output->add_clickable_plane(button_plane);

		auto text = std::make_shared<TextConsolePlane>(option.name, "resources/letters");
		text->foreground(' ', BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
		text->background(' ', BACKGROUND_BLUE | BACKGROUND_INTENSITY);
		text->position({ 0, 0 });
		button_plane->text(text);
	}

	logo_plane->position({
		output->width() / 2 - logo_plane->size().x / 2,
		output->height() / 2 - 14 });
	output->window_resize_event([logo_plane](auto new_width, auto new_height)
	{
		logo_plane->position({
			new_width / 2 - logo_plane->size().x / 2,
			new_height / 2 - 14});
	});
}
