#pragma once

#define WRITE_BUF_SZ 1024
#define MAX_WORKERS_THREADS 8

struct IAnimation;
struct IConsolePlane;
struct IClickableConsolePlane;

struct Console
{
private:
	Console();
	Console(const Console &console) = delete;

public:
	struct Buffer
	{
		friend struct Console;

		void clear(char chr, int color);
		void put(int x, int y, wchar_t chr, int color = FOREGROUND_INTENSITY);
		void put(int x, int y, char chr, int color = FOREGROUND_INTENSITY);

		int screen_width();
		int screen_height();

	private:
		Buffer(Console &);
		CHAR_INFO *_write_buf;
		Console &_console;
	};

	static std::shared_ptr<Console> get_instance();

	void exec();
	void stop();
	bool running();
	void active_screen(const char *buffer_name);
	void resize_window(int width, int height);
	void resolution(int sz);
	void clear_planes();

	int width() const;
	int height() const;

	void draw_async(std::function<void(const std::shared_ptr<Buffer> &buffer)>);
	void draw(const std::shared_ptr<Buffer> &buffer);
	std::shared_ptr<Buffer> create_buffer();
	void do_every(int period_ms, std::function<void()> trigger);
	void animate_async(const std::shared_ptr<IAnimation> &, int period_ms);
	void add_plane(const std::shared_ptr<IConsolePlane> &);
	void add_clickable_plane(const std::shared_ptr<IClickableConsolePlane> &);

	void mouse_click_event(
		std::function<void(int x, int y, int button, int flag)> callback);
	void window_resize_event(
		std::function<void(int new_width, int new_height)> callback);
	void _refresh_buffer_size();

	struct _Context
	{
		Console* console;
		HANDLE hStdIn;
		HANDLE hStdOutActive;
		std::map<const char *, HANDLE> hStdOutBack;
		std::function<void(int x, int y, int button, int flag)> mouse_click_callback;
		std::function<void(int new_width, int new_height)> window_resize_callback;
		bool mouse_clicked = false;
		COORD mouse_clicked_coords = { 0, 0 };
		std::vector<std::shared_ptr<IClickableConsolePlane>> clicked_clickable_planes;
		volatile int threadInputCounter;
		volatile int threadDrawCounter;
		volatile bool threadInputStop;
		volatile bool threadDrawStop;
	};

	struct _WorkerInfo
	{
		Console *console;
		PTP_WORK worker;
		std::function<void(const std::shared_ptr<Buffer> &)> callback;
		std::shared_ptr<Buffer> buffer;
		bool done;
	};

private:
	void _setup_threadpool();

	int _width;
	int _height;
	bool _running;

	_Context _context;
	PTP_POOL _thread_pool;
	TP_CALLBACK_ENVIRON _callback_environ;
	PTP_CLEANUP_GROUP _cleanup_group;
	HANDLE _workers_mutex;
	std::array<_WorkerInfo, MAX_WORKERS_THREADS> _workers;
	int _workers_next_index;

	std::vector<std::shared_ptr<IConsolePlane>> _planes;

public:
	void _thread_start();
	void _thread_stop();
	std::vector<std::shared_ptr<IClickableConsolePlane>> _clickable_planes;
};