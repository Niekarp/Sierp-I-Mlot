struct Menu
{
	struct Option
	{
		const char *name;
		std::function<void()> callback;
	};

	Menu();
	void add_option(const Option &);
	void draw_on(const std::shared_ptr<IOutput> &output);

private:
	std::vector<Option> _options;
};

auto main_menu = std::make_shared<Menu>();
main_menu->add_option({ "play", []
{

} });
main_menu->add_option({ "settings", []
{

} });
main_menu->add_option({ "exit", []
{

} });
// u¿ycie
main_menu->draw_on(IOutput);