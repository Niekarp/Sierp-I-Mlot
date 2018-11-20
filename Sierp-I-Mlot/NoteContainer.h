#pragma once


namespace xo
{
	class Note
	{
	public:
		long long time;
		unsigned tone;
		long long duration;
		bool clicked;
		long long destroy_time;
	};

	class NoteContainer
	{
	public:
		NoteContainer();
		void push(const Note &);
		void arrange();
		Note &operator[](size_t);
		Note &at(size_t);
		size_t size();		
		void clear();

	private:
		std::vector<Note> _notes;
		bool _arranged;
	};
}