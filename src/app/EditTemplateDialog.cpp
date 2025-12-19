#include "app/EditTemplateDialog.hpp"

EditTemplateDialog::EditTemplateDialog(
	Gtk::Window& parent,
	const std::string& languageName,
	const std::string& templateCode
) : Gtk::Dialog("My dialog", parent, true) {
	titleLabel.set_text("Редагування шаблону - " + languageName);

	auto buffer = textview.get_buffer();
	buffer->set_text(templateCode);

	setupUI();
}

std::string EditTemplateDialog::getTemplate() {
	auto buffer = textview.get_buffer();
	return buffer->get_text();
}

void EditTemplateDialog::setupUI() {
	set_default_size(WINDOW_WIDTH, WINDOW_HEIGHT);

	add_button("Cancel", Gtk::RESPONSE_CANCEL);
	add_button("Ok", Gtk::RESPONSE_OK);

	get_content_area()->pack_start(vbox);

	scrolled.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	textview.set_monospace(true);
	scrolled.add(textview);

	vbox.pack_start(titleLabel, Gtk::PACK_SHRINK);

	vbox.pack_start(scrolled, Gtk::PACK_EXPAND_WIDGET);

	show_all_children();
}