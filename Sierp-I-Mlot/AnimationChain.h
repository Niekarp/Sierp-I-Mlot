#pragma once
#include "IAnimation.h"

struct AnimationChain :
	public IAnimation
{
	AnimationChain();

	void draw(const std::shared_ptr<Console::Buffer> &buffer, size_t frame) override;
	bool end() override;
	bool continue_() override;
	
	void add(const std::shared_ptr<IAnimation> &animation);
	void overlap(size_t n_overlap_frames);
	void on_end(std::function<void(size_t animation_index)> callback);

private:
	struct AnimationMeta
	{
		std::shared_ptr<IAnimation> animation;
		size_t frame_offset;
	};

	std::vector<AnimationMeta> _animations;
	std::vector<AnimationMeta> _continued_animations;
	volatile size_t _current_animation_index;
	size_t _n_overlap_frames;
	volatile size_t _current_overlap_frame_index;
	std::function<void(size_t animation_index)> _on_end_callback;
	HANDLE _draw_mutex;
	bool _end;
	size_t _last_animation_frame;
};

