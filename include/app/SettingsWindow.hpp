#pragma once

#include <gtkmm.h>
#include <vector>
#include <core/LanguageTemplate.hpp>

class SettingsWindow : public Gtk::Window
{
public:
	SettingsWindow();

private:
	static constexpr int WINDOW_WIDTH = 400;
	static constexpr int WINDOW_HEIGHT = 400;

	Gtk::Box vbox{ Gtk::ORIENTATION_VERTICAL };
	Gtk::ListBox listbox;
	Gtk::Box buttonsBox{ Gtk::ORIENTATION_HORIZONTAL };
	Gtk::ScrolledWindow scrolled;

	Gtk::Button applyButton{ "Apply" };
	Gtk::Button cancelButton{ "Cancel" };

	std::vector<LanguageTemplate> languages;
	std::vector<LanguageTemplate> originalLanguages;

	void onApplyButtonClick();
	void onCancelButtonClick();
	void onEditButtonClick(size_t index);

	static bool hasChange();
	bool showConfirmationDialog(const Glib::ustring& title, const Glib::ustring& text);
	void renderLanguageRow(size_t index);
	void setupUI();

	void loadLanguages();
};
