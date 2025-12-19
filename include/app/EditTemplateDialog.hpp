#pragma once

#include <string>
#include <gtkmm.h>

class EditTemplateDialog : public Gtk::Dialog {
public:
	EditTemplateDialog(Gtk::Window& parent, const std::string& languageName, const std::string& templateCode);
	virtual ~EditTemplateDialog() = default;
	std::string getTemplate();

private:
	static constexpr int WINDOW_WIDTH = 400;
	static constexpr int WINDOW_HEIGHT = 400;

	Gtk::Box vbox{ Gtk::ORIENTATION_VERTICAL };
	Gtk::ScrolledWindow scrolled;

	Gtk::Label titleLabel;

	Gtk::TextView textview;

	void setupUI();
};