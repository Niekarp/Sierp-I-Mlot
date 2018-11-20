#pragma once
#include "XOIGameMapHero.h"
#include "IConsolePlane.h"
#include "FileImagePlane.h"
#include "HeroLifeBar.h"
#include "HeroProgressBarPlane.h"
#include "NoteContainer.h"

namespace xo
{
	const int N_STRINGS = 4;

	class XOConsoleGameMapHero :
		public XOIGameMapHero,
		public IConsolePlane,
		public std::enable_shared_from_this<XOConsoleGameMapHero>
	{
	public:
		XOConsoleGameMapHero();
		~XOConsoleGameMapHero();

		void register_element(const XOViewElement &) override;
		void start(long long start, long long omitt, long long duration) override;


		// Inherited via XOIGameMapHero
		Position position() override;
		Position size() override;
		PlaneType type() override;
		void draw(const std::shared_ptr<Console::Buffer>&) override;
		void button(int string_n, bool pressed) override;
		void progress(float) override;
		void life(float) override;
		void reset() override;
		void player_symbol(xo::PlayerSymbol) override;
		void feed(const std::shared_ptr<NoteContainer>& notes) override;

		void draw_on(const std::shared_ptr<Console> &);

	private:
		void _on_key(int key, bool down);

		std::shared_ptr<HeroLifeBar> _life_bar;
		std::shared_ptr<HeroProgressBarPlane> _progress_bar;
		std::shared_ptr<FileImagePlane> _note_image;
		std::shared_ptr<NoteContainer> _notes;
		std::chrono::system_clock::time_point _start_point;
		std::array<bool, N_STRINGS> _button_states;
		std::vector<XOViewElement> _elements;
		std::function<void(int key)> _key_down_callback;
		std::function<void(int key)> _key_up_callback;
		bool _key_callback_binded;
		int _frame;
		long long _omission_time;
		long long _duration;
		bool _play;
		xo::PlayerSymbol _player_symbol;
	};
}

