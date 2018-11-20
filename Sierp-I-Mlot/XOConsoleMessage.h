#pragma once
#include "XOIMessage.h"
#include "IAnimation.h"
#include "Console.h"
#include "XOViewElementRegistrar.h"
#include "TextConsolePlane.h"

namespace xo
{
	class XOConsoleMessage : 
		public XOIMessage, 
		public XOViewElementRegistrar,
		public IAnimation,
		public std::enable_shared_from_this<XOConsoleMessage>
	{
	public:
		XOConsoleMessage();

		// Inherited via XOIMessage
		virtual void text(const std::string & msg) override;
		virtual void time(size_t ms) override;

		// Inherited via IAnimation
		virtual void draw(const std::shared_ptr<Console::Buffer>& buffer, size_t frame) override;
		virtual bool end() override;
		virtual bool continue_() override;
		virtual void type(Type) override;

		void draw_on(const std::shared_ptr<Console> &);

	private:
		std::shared_ptr<TextConsolePlane> _text_plane;
		std::string _msg;
		size_t _time_ms;
		bool _end;
		Type _type;
	};
}