#pragma once
#include "IAnimation.h"
#include "FileImagePlane.h"
#include "AnimatedFileImagePlane.h"

struct IntroAnimation :
	public IAnimation
{
	IntroAnimation(
		const std::shared_ptr<FileImagePlane> &image1,
		const std::shared_ptr<FileImagePlane> &image2,
		const std::shared_ptr<FileImagePlane> &image3,
		const std::shared_ptr<AnimatedFileImagePlane> &animated_image4);
	void draw(const std::shared_ptr<Console::Buffer> &buffer, size_t frame) override;
	bool end() override;
	bool continue_() override;

private:
	std::shared_ptr<FileImagePlane> _image1;
	std::shared_ptr<FileImagePlane> _image2;
	std::shared_ptr<FileImagePlane> _image3;
	std::shared_ptr<AnimatedFileImagePlane> _animated_image4;
	bool _end;
};

