#include "pch.h"
#include "Console.h"
#include "IAnimation.h"
#include "IConsolePlane.h"
#include "IClickableConsolePlane.h"


static VOID WINAPI _SetupConsole(Console::_Context *context);
static DWORD WINAPI _ConsoleInputRoutine(Console::_Context &context);
static VOID WINAPI _OpenConsoleInputRoutine(Console::_Context &context);
static BOOLEAN WINAPI _ConsoleInputRecordCallback(Console::_Context & context,
	const INPUT_RECORD& record);
static VOID CALLBACK _ConsoleWorkCallback(PTP_CALLBACK_INSTANCE instance,
	PVOID arg,
	PTP_WORK work);

Console::Buffer::Buffer(Console &console) :
	_write_buf(new CHAR_INFO[WRITE_BUF_SZ * WRITE_BUF_SZ]),
	_console(console)
{
	clear(' ', 0);
	ZeroMemory(_write_buf, sizeof(*_write_buf) * WRITE_BUF_SZ * WRITE_BUF_SZ);
}

void Console::Buffer::put(int x, int y, wchar_t chr,
	int color)
{
	_write_buf[y * WRITE_BUF_SZ + x].Char.UnicodeChar = chr;
	_write_buf[y * WRITE_BUF_SZ + x].Attributes = color;
}

void Console::Buffer::put(int x, int y, char chr,
	int color)
{
	if (x < 0 || x >= WRITE_BUF_SZ)
	{
		return;
	}
	if (y < 0 || y >= WRITE_BUF_SZ)
	{
		return;
	}
	_write_buf[y * WRITE_BUF_SZ + x].Char.UnicodeChar = 0;
	_write_buf[y * WRITE_BUF_SZ + x].Char.AsciiChar = chr;
	_write_buf[y * WRITE_BUF_SZ + x].Attributes = color;
}

int Console::Buffer::screen_width()
{
	return _console.width();
}

int Console::Buffer::screen_height()
{
	return _console.height();
}

Console::Console() :
	_context({ this }),
	_width(0),
	_height(0),
	_workers_next_index(0),
	_running(false)
{
	_context.threadInputCounter = 0;
	_context.threadDrawCounter = 0;
	_context.threadInputStop = false;
	_context.threadDrawStop = false;
	_SetupConsole(&_context);
	_setup_threadpool();
}

void Console::_setup_threadpool()
{
	InitializeThreadpoolEnvironment(&_callback_environ);
	CHKERR_PVOID(_thread_pool = CreateThreadpool(NULL));
	CHKERR_BOOL(SetThreadpoolThreadMinimum(_thread_pool, MAX_WORKERS_THREADS));
	CHKERR_BOOL(SetThreadpoolThreadMinimum(_thread_pool, MAX_WORKERS_THREADS));
	CHKERR_PVOID(_cleanup_group = CreateThreadpoolCleanupGroup());
	SetThreadpoolCallbackPool(&_callback_environ, _thread_pool);
	SetThreadpoolCallbackCleanupGroup(&_callback_environ,
		_cleanup_group, NULL);

	for (auto &worker_info : _workers)
	{
		CHKERR_PVOID(worker_info.worker = CreateThreadpoolWork(_ConsoleWorkCallback,
			&worker_info, &_callback_environ));
		worker_info.buffer = create_buffer();
		worker_info.console = this;
		worker_info.done = true;
	}

	_workers_mutex = CreateMutex(NULL, FALSE, NULL);
}

void Console::_thread_start()
{
	_context.threadDrawStop = false;
	_context.threadInputStop = false;
	CHKERR_BOOL(ReleaseMutex(_workers_mutex));
}

void Console::_thread_stop()
{
	CHKERR_DWORD(WaitForSingleObject(_workers_mutex, INFINITE));
	_context.threadInputStop = true;
	_context.threadDrawStop = true;
	while (_context.threadInputCounter > 1)
	{
		Sleep(1);
	}
	while (_context.threadDrawCounter > 0)
	{
		Sleep(1);
	}
}

std::shared_ptr<Console> Console::get_instance()
{
	static std::shared_ptr<Console> console(new Console());
	return console;
}

void Console::draw(const std::shared_ptr<Buffer> &buffer)
{
	if (_context.threadDrawStop)
	{
		return;
	}
	_context.threadDrawCounter += 1;

	for (auto &plane : _planes)
	{
		plane->draw(buffer);
	}

	for (auto &plane : _clickable_planes)
	{
		plane->draw(buffer);
	}

	SMALL_RECT drawRect{};
	drawRect.Right = _width;
	drawRect.Bottom = _height;
	CHKERR_BOOL(WriteConsoleOutputA(_context.hStdOutActive,
		buffer->_write_buf, COORD{ WRITE_BUF_SZ, WRITE_BUF_SZ },
		COORD{ 0,0 }, &drawRect));

	_context.threadDrawCounter -= 1;
}

std::shared_ptr<Console::Buffer> Console::create_buffer()
{
	return std::shared_ptr<Buffer>(new Buffer(*this));
}

void Console::exec()
{
	_running = true;
	_OpenConsoleInputRoutine(_context);
}

void Console::stop()
{
	_running = false;
}

bool Console::running()
{
	return _running;
}

void Console::_refresh_buffer_size()
{
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
	BOOL result = FALSE;

	do
	{
		CHKERR_BOOL(GetConsoleScreenBufferInfo(_context.hStdOutActive, &screenBufferInfo));

		_width = screenBufferInfo.srWindow.Right - screenBufferInfo.srWindow.Left + 1;
		_height = screenBufferInfo.srWindow.Bottom - screenBufferInfo.srWindow.Top + 1;

		result = SetConsoleScreenBufferSize(_context.hStdOutActive, { (SHORT)_width,  (SHORT)_height });
	} while (!result);
}

void Console::active_screen(const char *buffer_name)
{
	if (_context.hStdOutBack.find(buffer_name) == _context.hStdOutBack.end())
	{
		CHKERR_HANDLE(_context.hStdOutBack[buffer_name] = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE, 0, NULL, 0, NULL));
	}

	_context.hStdOutActive = _context.hStdOutBack[buffer_name];
	CHKERR_BOOL(SetConsoleActiveScreenBuffer(_context.hStdOutActive));
}

void Console::resize_window(int width, int height)
{
	HWND hConsole = GetConsoleWindow();
	RECT consoleSize;
	CHKERR_BOOL(GetWindowRect(hConsole, &consoleSize));

	CHKERR_BOOL(MoveWindow(hConsole, consoleSize.left, consoleSize.top,
		width, height, TRUE));

	_refresh_buffer_size();
}

void Console::resolution(int sz)
{
	CONSOLE_FONT_INFOEX consoleFont;
	consoleFont.cbSize = sizeof(consoleFont);
	CHKERR_BOOL(GetCurrentConsoleFontEx(_context.hStdOutActive, FALSE, &consoleFont));

	consoleFont.dwFontSize = COORD{ 0, (SHORT)sz };
	//consoleFont.FaceName;
	CHKERR_BOOL(SetCurrentConsoleFontEx(_context.hStdOutActive, FALSE, &consoleFont));
}

void Console::clear_planes()
{
	_thread_stop();

	_context.mouse_clicked = false;
	_planes.clear();
	_clickable_planes.clear();
	
	_thread_start();
}

void Console::Buffer::clear(char chr, int color)
{
	CHAR_INFO chr_info{};
	chr_info.Char.AsciiChar = chr;
	chr_info.Attributes = color;

	for (int i = 0; i < WRITE_BUF_SZ * WRITE_BUF_SZ; ++i)
	{
		_write_buf[i] = chr_info;
	}
	/*DWORD written;
	CHKERR(FillConsoleOutputCharacter(_context.hStdOutActive,
		(TCHAR)chr,
		_width * _height,
		{ 0, 0 },
		&written));*/
}

int Console::width() const
{
	return _width;
}

int Console::height() const
{
	return _height;
}

void Console::draw_async(
	std::function<void(const std::shared_ptr<Buffer> &buffer)> callback)
{
	WaitForSingleObject(_workers_mutex, INFINITE);
	for (;;)
	{
		auto &worker_info = _workers[_workers_next_index];
		_workers_next_index = (_workers_next_index + 1) % _workers.size();
		if (worker_info.done)
		{
			worker_info.done = false;
			worker_info.callback = callback;
			SubmitThreadpoolWork(worker_info.worker);
			break;
		}
	}
	CHKERR_BOOL(ReleaseMutex(_workers_mutex));
}

void Console::do_every(int period_ms, std::function<void()> trigger)
{
	static std::function<void()> ownTrigger = trigger;
	HANDLE timer;
	CHKERR_BOOL(CreateTimerQueueTimer(&timer, NULL, [](PVOID trigger, BOOLEAN towf)
	{
		ownTrigger();
	}, NULL, period_ms, period_ms, WT_EXECUTEDEFAULT));
}

void Console::animate_async(const std::shared_ptr<IAnimation>& animation,
	int period_ms)
{
	do_every(period_ms, [&, animation] {
		draw_async([&, animation](auto buffer)
		{
			animation->draw(buffer);
			draw(buffer);
		});
	});
}

void Console::add_plane(const std::shared_ptr<IConsolePlane> &plane)
{
	_planes.push_back(plane);
}

void Console::add_clickable_plane(const std::shared_ptr<IClickableConsolePlane> &plane)
{
	_thread_stop();

	_clickable_planes.push_back(plane);
	
	_thread_start();
}

void Console::mouse_click_event(
	std::function<void(int x, int y, int button, int flag)> callback)
{
	_context.mouse_click_callback = callback;
}

void Console::window_resize_event(
	std::function<void(int new_width, int new_height)> callback)
{
	_context.window_resize_callback = callback;
}

static VOID WINAPI _SetupConsole(Console::_Context *context)
{
	CHKERR_BOOL(SetConsoleOutputCP(852));
	CHKERR_HANDLE(context->hStdIn = CreateFile(L"CONIN$",
		GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL));

	CHKERR_HANDLE(context->hStdOutActive = CreateFile(L"CONOUT$",
		GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL));

	context->hStdOutBack["main"] = context->hStdOutActive;

	DWORD consoleOutputMode;
	CHKERR_BOOL(GetConsoleMode(context->hStdOutActive, &consoleOutputMode));

	consoleOutputMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	CHKERR_BOOL(SetConsoleMode(context->hStdOutActive, consoleOutputMode));

	DWORD consoleInputMode;
	CHKERR_BOOL(GetConsoleMode(context->hStdIn, &consoleInputMode));

	CHKERR_BOOL(SetConsoleMode(context->hStdIn, ENABLE_EXTENDED_FLAGS));

	consoleInputMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	CHKERR_BOOL(SetConsoleMode(context->hStdIn, consoleInputMode));
}

//DWORD WINAPI ConsoleInputRoutine(LPVOID arg)
static DWORD WINAPI _ConsoleInputRoutine(Console::_Context &context)
{
	while (context.console->running())
	{
		CHKERR_DWORD(WaitForSingleObject(context.hStdIn, INFINITE));

		DWORD numOfEvents;
		CHKERR_BOOL(GetNumberOfConsoleInputEvents(context.hStdIn, &numOfEvents));

		if (numOfEvents == 0)
		{
			continue;
		}

		auto inputRecords = std::make_unique<INPUT_RECORD[]>(numOfEvents);
		DWORD numOfEventsRead;
		CHKERR_BOOL(ReadConsoleInput(context.hStdIn, inputRecords.get(), numOfEvents, &numOfEventsRead));

		BOOLEAN resized = FALSE;
		for (DWORD i = 0; i < numOfEventsRead; ++i)
		{
			resized |= _ConsoleInputRecordCallback(context, inputRecords[i]);
		}

		if (resized)
		{
			context.console->_refresh_buffer_size();
		}
	}

	return 0;
}

static VOID WINAPI _OpenConsoleInputRoutine(Console::_Context &context)
{
	_ConsoleInputRoutine(context);
}

static BOOLEAN WINAPI _HandleConsoleInputRecord(Console::_Context &context, const INPUT_RECORD& record)
{
	if (record.EventType == MOUSE_EVENT)
	{
		if (!record.Event.MouseEvent.dwButtonState)
		{
			if (context.mouse_clicked)
			{
				for (int i = 0; i < context.clicked_clickable_planes.size(); ++i)
				{
					auto clickable_plane = context.clicked_clickable_planes[i];
					clickable_plane->click_release();
				}
				context.clicked_clickable_planes.clear();
				context.mouse_clicked = false;
			}
			return false;
		}
		context.mouse_clicked = true;
		context.mouse_clicked_coords = record.Event.MouseEvent.dwMousePosition;

		for(int i = 0; i < context.console->_clickable_planes.size(); ++i)
		{
			auto clickable_plane = context.console->_clickable_planes[i];
			
			if (clickable_plane->type() == IConsolePlane::PlaneType::CENTERED)
			{
				auto plane_pos = clickable_plane->position();
				auto plane_sz = clickable_plane->size();
				auto bx = plane_pos.x - plane_sz.x / 2;
				auto by = plane_pos.y - plane_sz.y / 2;
				auto ex = plane_pos.x + plane_sz.x / 2;
				auto ey = plane_pos.y + plane_sz.y / 2;

				if (context.mouse_clicked_coords.X >= bx && context.mouse_clicked_coords.X < ex
					&& context.mouse_clicked_coords.Y >= by && context.mouse_clicked_coords.Y < ey)
				{
					clickable_plane->click({ (int)context.mouse_clicked_coords.X,
						(int)context.mouse_clicked_coords.Y }, record.Event.MouseEvent.dwButtonState,
						record.Event.MouseEvent.dwEventFlags);
					context.clicked_clickable_planes.push_back(clickable_plane);
				}
			}
		}
		if (context.mouse_click_callback)
		{
			context.mouse_click_callback(
				(int)record.Event.MouseEvent.dwMousePosition.X,
				(int)record.Event.MouseEvent.dwMousePosition.Y,
				(int)record.Event.MouseEvent.dwButtonState,
				(int)record.Event.MouseEvent.dwEventFlags);
		}
	}
	else if (record.EventType == WINDOW_BUFFER_SIZE_EVENT)
	{
		if (context.window_resize_callback == nullptr)
		{
			return true;
		}

		auto newSize = record.Event.WindowBufferSizeEvent.dwSize;
		context.window_resize_callback((int)newSize.X, (int)newSize.Y);

		return true;
	}

	return false;
}

static BOOLEAN WINAPI _ConsoleInputRecordCallback(Console::_Context &context,
	const INPUT_RECORD& record)
{
	if (context.threadInputStop)
	{
		return false;
	}
	
	context.threadInputCounter += 1;
	auto result = _HandleConsoleInputRecord(context, record);
	context.threadInputCounter -= 1;
	
	return result;
}
VOID CALLBACK _ConsoleWorkCallback(PTP_CALLBACK_INSTANCE instance,
	PVOID arg,
	PTP_WORK work)
{
	auto worker_info = (Console::_WorkerInfo *)arg;
	worker_info->callback(worker_info->buffer);
	worker_info->console->draw(worker_info->buffer);
	worker_info->done = true;
}