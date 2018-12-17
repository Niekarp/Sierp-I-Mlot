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

	class NoteContainer :
		std::enable_shared_from_this<NoteContainer>
	{
	public:
		NoteContainer();
		void push(const Note &);
		void arrange();
		void clear();

		class iterator
		{
		private:
			size_t _index;
			std::shared_ptr<NoteContainer> _container;
			class noteholder
			{
				size_t _index;
				std::shared_ptr<NoteContainer> _container;
			public:
				noteholder(const std::shared_ptr<NoteContainer> &container, size_t index) :
					_container(container),
					_index(index)
				{}
				Note &operator*()
				{
					if (!_container->_arranged)
					{
						_container->arrange();
					}
					return _container->_notes[_index];
				}
			};
			explicit iterator(const std::shared_ptr<NoteContainer> &container, size_t index) :
				_container(container),
				_index(index)
			{}
		public:
			friend class NoteContainer;

			typedef int                     value_type;
			typedef std::ptrdiff_t          difference_type;
			typedef int*                    pointer;
			typedef int&                    reference;
			typedef std::input_iterator_tag iterator_category;

			Note &operator*() const
			{
				if (!_container->_arranged)
				{
					_container->arrange();
				}
				return _container->_notes[_index];
			}
			bool operator==(const iterator& other) const
			{
				return _index == other._index;
			}
			bool operator!=(const iterator& other) const { return !(*this == other); }
			noteholder operator++(int)
			{
				noteholder ret(_container, _index);
				++*this;
				return ret;
			}
			iterator& operator++()
			{
				++_index;
				return *this;
			}
		};


		iterator begin();
		iterator end();


	private:
		std::vector<Note> _notes;
		bool _arranged;
	};
}