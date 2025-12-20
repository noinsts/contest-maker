#include "app/EditTemplateDialog.hpp"

EditTemplateDialog::EditTemplateDialog(
	Gtk::Window& parent,
	const std::string& languageName,
	const std::string& templateCode
) : Gtk::Dialog("Редагування шаблону", parent, true) {
	Gsv::init();

	titleLabel.set_markup("<b>Редагування шаблону</b> " + languageName);

	auto lang_manager = Gsv::LanguageManager::get_default();
	auto lang = lang_manager->guess_language("dummy" + languageName, "");

	sourceBuffer = Gsv::Buffer::create(lang);
	sourceBuffer->set_text(templateCode);

	auto scheme_manager = Gsv::StyleSchemeManager::get_default();
	sourceBuffer->set_style_scheme(scheme_manager->get_scheme("solarized-dark"));

	sourceView.set_buffer(sourceBuffer);

	setupUI();
}

std::string EditTemplateDialog::getTemplate() {
	return sourceBuffer->get_text();
}

void EditTemplateDialog::setupUI() {
	set_default_size(WINDOW_WIDTH, WINDOW_HEIGHT);

	add_button("Cancel", Gtk::RESPONSE_CANCEL);
	add_button("Ok", Gtk::RESPONSE_OK);

	auto content_area = get_content_area();
	content_area->set_border_width(12);
	content_area->pack_start(vbox);

	titleLabel.set_halign(Gtk::ALIGN_START);
	titleLabel.set_margin_bottom(10);
	titleLabel.set_margin_top(5);

	sourceView.set_tab_width(4);
	sourceView.set_insert_spaces_instead_of_tabs(false);
	sourceView.set_show_line_numbers(true);
	sourceView.set_highlight_current_line(true);
	sourceView.set_monospace(true);
	sourceView.set_auto_indent(true);

	scrolled.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	scrolled.set_shadow_type(Gtk::SHADOW_IN);
	scrolled.set_margin_bottom(10);
	scrolled.add(sourceView);

	vbox.set_spacing(6);
	vbox.pack_start(titleLabel, Gtk::PACK_SHRINK);
	vbox.pack_start(scrolled, Gtk::PACK_EXPAND_WIDGET);

	show_all_children();
}