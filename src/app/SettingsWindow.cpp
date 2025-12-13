#include "app/SettingsWindow.hpp"

SettingsWindow::SettingsWindow() {
	setupUI();
}

void SettingsWindow::setupUI() {
	set_title("Налаштування шаблонів");
	set_default_size(WINDOW_WIDTH, WIDTH_HEIGHT);

	scrolled.add(label);

	scrolled.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	add(scrolled);
	show_all();
}