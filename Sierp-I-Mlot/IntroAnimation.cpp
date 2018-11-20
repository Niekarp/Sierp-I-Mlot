#include "pch.h"
#include "IntroAnimation.h"

IntroAnimation::IntroAnimation(
	const std::shared_ptr<FileImagePlane> &image1,
	const std::shared_ptr<FileImagePlane> &image2,
	const std::shared_ptr<FileImagePlane> &image3,
	const std::shared_ptr<AnimatedFileImagePlane> &animated_image4) :
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

void IntroAnimation::draw(const std::shared_ptr<Console::Buffer>& buffer, size_t frame)
{
	buffer->clear(' ', 0);

	auto cx = buffer->screen_width() / 2;
	auto cy = buffer->screen_height() / 2;

	const auto frame_key1 = 150;
	const auto frame_key2 = frame_key1 + 10;
	const auto frame_key3 = frame_key2 + 45;
	const auto frame_key4 = frame_key3 + 0;
	const auto frame_key5 = frame_key4 + 105;
	const auto frame_key6 = frame_key5 + 80;

	frame = 3 * frame / 2;

	// key 1
	if (frame < frame_key1)
	{
		_image1->position(
		{
			cx - _image1->size().x / 2 + (int)frame - 150,
			cy - _image1->size().y / 2 - 1
		});
		_image2->position(
		{
			cx - _image2->size().x / 2 - (int)frame + 150,
			cy + _image2->size().y / 2 + 1
		});
	}
	// key 2
	else if (frame < frame_key2)
	{
		_image1->position(
		{
			cx - _image1->size().x / 2 + frame_key1 - 150,
			cy - _image1->size().y / 2 - 1 - (int)(frame - frame_key1) / 2
		});
		_image2->position(
		{
			cx - _image2->size().x / 2 - frame_key1 + 150,
			cy + _image2->size().y / 2 + 1 - (int)(frame - frame_key1) / 2
		});
	}
	// key 3
	else if (frame < frame_key3)
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
	}
	// key 4
	else if (frame < frame_key4)
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
	}
	// key 5
	else if((frame - frame_key4) / 6 <= _animated_image4->last_frame_index())
	{
		_animated_image4->frame((frame - frame_key4) / 6);
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
		_animated_image4->position(
		{
			cx - _animated_image4->size().x / 2,
			cy - _animated_image4->size().y / 2 - 20
		});	
	}
	else if (frame < frame_key5)
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
			cy + _image3->size().y / 2 + 75 - (int)(2 * (frame - frame_key4) / 3)
		});
	}
	else if(frame < frame_key6)
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
			cy + _image3->size().y / 2 + 75 - 2 * (frame_key5 - frame_key4) / 3
		});
		_animated_image4->position(
		{
			cx - _animated_image4->size().x / 2,
			cy - _animated_image4->size().y / 2 - 20
		});
		/*
				_image3->position(
		{
			cx - _image3->size().x / 2,
			cy + _image3->size().y / 2 + 50 - (int)(frame - frame_key3)
		});*/
	}
	else
	{
		_end = true;
	}

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
