#pragma once
#include "IAnimation.h"

struct AnimationChain :
	public IAnimation
{
	AnimationChain(int animation_index = 0);

	void draw(const std::shared_ptr<Console::Buffer> &buffer) override;
	bool end() override;
	bool continue_() override;
	
	void add(const std::shared_ptr<IAnimation> &animation);
	void overlap(size_t n_overlap_frames);
	void on_end(std::function<void(size_t animation_index)> callback);

private:
	std::vector<std::shared_ptr<IAnimation>> _animations;
	std::vector<std::shared_ptr<IAnimation>> _continued_animations;
	volatile size_t _current_animation_index;
	size_t _n_overlap_frames;
	volatile size_t _current_overlap_frame_index;
	std::function<void(size_t animation_index)> _on_end_callback;
	HANDLE _draw_mutex;
	bool _end;
};

