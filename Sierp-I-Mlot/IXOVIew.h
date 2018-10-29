#pragma once

class IXOView
{
public:
	virtual void register_element(const char *id, const char *text,
		std::function<void()> callback) = 0;
	virtual void register_element(const char *id,
		std::function<void()> callback) { register_element(id, "", callback); }
};