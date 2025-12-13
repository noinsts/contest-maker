#pragma once

#include <gtkmm.h>

class SettingsWindow : public Gtk::Window
{
public:
	SettingsWindow();

private:
	static constexpr int WINDOW_WIDTH = 400;
	static constexpr int WIDTH_HEIGHT = 400;

	Gtk::ScrolledWindow scrolled;

	Gtk::Label label{ "Hello, world!" };

	void setupUI();
};
