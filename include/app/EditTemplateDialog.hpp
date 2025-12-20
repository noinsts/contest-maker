#pragma once

#include <string>
#include <gtkmm.h>
#include <gtksourceviewmm.h>

class EditTemplateDialog : public Gtk::Dialog {
public:
	EditTemplateDialog(Gtk::Window& parent, const std::string& languageName, const std::string& templateCode);
	virtual ~EditTemplateDialog() = default;
	std::string getTemplate();

private:
	static constexpr int WINDOW_WIDTH = 600;
	static constexpr int WINDOW_HEIGHT = 500;

	Gtk::Box vbox{ Gtk::ORIENTATION_VERTICAL };
	Gtk::ScrolledWindow scrolled;

	Gtk::Label titleLabel;

	Gsv::View sourceView;
	Glib::RefPtr<Gsv::Buffer> sourceBuffer;

	void setupUI();
};