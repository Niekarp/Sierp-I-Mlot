#include "pch.h"
#include "AnimationChain.h"

AnimationChain::AnimationChain() :
	_n_overlap_frames(0),
	_current_overlap_frame_index(0),
	_end(false),
	_current_animation_index(0)
{
	_draw_mutex = CreateMutex(NULL, FALSE, NULL);
}

void AnimationChain::draw(const std::shared_ptr<Console::Buffer>& buffer, size_t frame)
{
	//CHKERR_DWORD(WaitForSingleObject(_draw_mutex, INFINITE));

	_continued_animations.erase(std::remove_if(
		_continued_animations.begin(), _continued_animations.end(), [&](auto &e) {
		e.animation->draw(buffer, frame);
		if (e.animation->end())
		{
			/*if (_on_end_callback)
			{
				_on_end_callback(_current_animation_index);
			}*/
			return true;
		}
		return false;
	}), _continued_animations.end());

	if (_current_animation_index >= _animations.size())
	{
		return;
	}

	auto &current_animation = _animations[_current_animation_index];

	if (_current_overlap_frame_index < _n_overlap_frames
		&& _current_animation_index != 0)
	{

		auto &prev_animation = _animations[_current_animation_index - 1];
		_current_overlap_frame_index += 1;
		prev_animation.animation->draw(buffer, 
			frame - prev_animation.frame_offset);
	}

	current_animation.animation->draw(buffer, 
		frame - current_animation.frame_offset);

	if (_end)
	{
		return;
	}

	CHKERR_DWORD(WaitForSingleObject(_draw_mutex, INFINITE));

	if (_end)
	{
		return;
	}

	if (current_animation.animation->continue_())
	{
		_continued_animations.push_back(current_animation);
		_current_animation_index += 1;
		_current_overlap_frame_index = 0;
		
		if (_current_animation_index >= _animations.size())
		{
			_end = true;
		}
		else
		{
			_animations[_current_animation_index].frame_offset = frame;
		}		
	}
	else if (current_animation.animation->end())
	{
		if (_on_end_callback)
		{
			_on_end_callback(_current_animation_index);
		}
		_current_animation_index += 1;
		_current_overlap_frame_index = 0;
		
		if (_current_animation_index >= _animations.size())
		{
			_end = true;
		}
		else
		{
			_animations[_current_animation_index].frame_offset = frame;
		}
	}

	CHKERR_BOOL(ReleaseMutex(_draw_mutex));
}

bool AnimationChain::end()
{
	return _current_animation_index == _animations.size() 
		&& _animations.back().animation->end();
}

bool AnimationChain::continue_()
{
	return false;
}

void AnimationChain::add(const std::shared_ptr<IAnimation>& animation)
{
	_animations.push_back({ animation, 0 });
}

void AnimationChain::overlap(size_t n_overlap_frames)
{
	_n_overlap_frames = n_overlap_frames;
}

void AnimationChain::on_end(std::function<void(size_t animation_index)> callback)
{
	_on_end_callback = callback;
}