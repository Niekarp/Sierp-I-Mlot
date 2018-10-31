#include "pch.h"
#include "AnimationChain.h"

AnimationChain::AnimationChain() :
	_n_overlap_frames(0),
	_current_overlap_frame_index(0),
	_current_animation_index(0)
{
}

void AnimationChain::draw(const std::shared_ptr<Console::Buffer>& buffer)
{
	if (_current_overlap_frame_index < _n_overlap_frames
		&& _current_animation_index != 0
		&& _current_animation_index < _animations.size())
	{
		_current_overlap_frame_index += 1;
		_animations[_current_animation_index - 1]->draw(buffer);
	}

	_continued_animations.erase(std::remove_if(
		_continued_animations.begin(), _continued_animations.end(), [&](auto &e) {
		e->draw(buffer);
		if (e->end())
		{
			if (_on_end_callback)
			{
				_on_end_callback(_current_animation_index);
			}
			return true;
		}
		return false;
	}), _continued_animations.end());

	if (_current_animation_index < _animations.size())
	{
		_animations[_current_animation_index]->draw(buffer);

		if (_animations[_current_animation_index]->continue_())
		{
			_continued_animations.push_back(_animations[_current_animation_index]);
			_current_animation_index += 1;
			_current_overlap_frame_index = 0;
		}
		else if (_animations[_current_animation_index]->end())
		{
			if (_on_end_callback)
			{
				_on_end_callback(_current_animation_index);
			}
			_current_animation_index += 1;
			_current_overlap_frame_index = 0;
		}
	}
}

bool AnimationChain::end()
{
	return _current_animation_index == _animations.size() && _animations.back()->end();
}

bool AnimationChain::continue_()
{
	return false;
}

void AnimationChain::add(const std::shared_ptr<IAnimation>& animation)
{
	_animations.push_back(animation);
}

void AnimationChain::overlap(size_t n_overlap_frames)
{
	_n_overlap_frames = n_overlap_frames;
}

void AnimationChain::on_end(std::function<void(size_t animation_index)> callback)
{
	_on_end_callback = callback;
}