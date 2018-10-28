#include "../pch.h"
#include "IntroAnimation.h"

IntroAnimation::IntroAnimation(
	const std::shared_ptr<FileImagePlane> &image1,
	const std::shared_ptr<FileImagePlane> &image2,
	const std::shared_ptr<FileImagePlane> &image3,
	const std::shared_ptr<AnimatedFileImagePlane> &animated_image4) :
	_frame_index(0),
	_end(false)
{
	_image1 = image1;
	_image1->position({ -1000, -1000 });

	_image2 = image2;
	_image2->position({ -1000, -1000 });

	_image3 = image3;
	_image3->position({ -1000, -1000 });


	_animated_image4 = animated_image4;
	_animated_image4->position({ -1000, -1000 });
}

void IntroAnimation::draw(const std::shared_ptr<Console::Buffer>& buffer)
{
	buffer->clear(' ', 0);

	auto cx = buffer->screen_width() / 2;
	auto cy = buffer->screen_height() / 2;

	const auto frame_key1 = 150;
	const auto frame_key2 = 160;
	const auto frame_key3 = 205;
	const auto frame_key4 = 320;

	// key 1
	if (_frame_index < frame_key1)
	{
		_image1->position(
		{
			cx - _image1->size().x / 2 + (int)_frame_index - 150,
			cy - _image1->size().y / 2 - 1
		});
		_image2->position(
		{
			cx - _image2->size().x / 2 - (int)_frame_index + 150,
			cy + _image2->size().y / 2 + 1
		});
	}
	// key 2
	else if (_frame_index < frame_key2)
	{
		_image1->position(
		{
			cx - _image1->size().x / 2 + frame_key1 - 150,
			cy - _image1->size().y / 2 - 1 - (int)(_frame_index - frame_key1) / 2
		});
		_image2->position(
		{
			cx - _image2->size().x / 2 - frame_key1 + 150,
			cy + _image2->size().y / 2 + 1 - (int)(_frame_index - frame_key1) / 2
		});
	}
	// key 3
	else if (_frame_index < frame_key3)
	{
		_image1->position(
		{
			cx - _image1->size().x / 2 + frame_key1 - 150,
			cy - _image1->size().y / 2 - 1 - (frame_key2 - frame_key1) / 2
		});
		_image2->position(
		{
			cx - _image2->size().x / 2 - frame_key1 + 150,
			cy + _image2->size().y / 2 + 1 - (frame_key2 - frame_key1) / 2
		});
		_image3->position(
		{
			cx - _image3->size().x / 2,
			cy + _image3->size().y / 2 + 50 - (int)(_frame_index - frame_key2)
		});
	}
	// key 4
	else if(_animated_image4->frame_index() < _animated_image4->end_frame_index())
	{
		_image1->position(
		{
			cx - _image1->size().x / 2 + frame_key1 - 150,
			cy - _image1->size().y / 2 - 1 - (frame_key2 - frame_key1) / 2
		});
		_image2->position(
		{
			cx - _image2->size().x / 2 - frame_key1 + 150,
			cy + _image2->size().y / 2 + 1 - (frame_key2 - frame_key1) / 2
		});
		_image3->position(
		{
			cx - _image3->size().x / 2,
			cy + _image3->size().y / 2 + 50 - (frame_key3 - frame_key2)
		});
		_animated_image4->position(
		{
			cx - _animated_image4->size().x / 2,
			cy - _animated_image4->size().y / 2 - 22
		});
		if (_frame_index % 8 == 0)
		{
			_animated_image4->next();
		}		
	}
	else if(_frame_index < frame_key4)
	{
		_image1->position(
		{
			cx - _image1->size().x / 2 + frame_key1 - 150,
			cy - _image1->size().y / 2 - 1 - (frame_key2 - frame_key1) / 2
		});
		_image2->position(
		{
			cx - _image2->size().x / 2 - frame_key1 + 150,
			cy + _image2->size().y / 2 + 1 - (frame_key2 - frame_key1) / 2
		});
		_image3->position(
		{
			cx - _image3->size().x / 2,
			cy + _image3->size().y / 2 + 50 - (frame_key3 - frame_key2)
		});
		_animated_image4->position(
		{
			cx - _animated_image4->size().x / 2,
			cy - _animated_image4->size().y / 2 - 22
		});
		
	}
	else
	{
		_end = true;
	}
	_frame_index += 2;

	_image1->draw(buffer);
	_image2->draw(buffer);
	_image3->draw(buffer);
	_animated_image4->draw(buffer);
}

bool IntroAnimation::end()
{
	return _end;
}

bool IntroAnimation::continue_()
{
	return false;
}
