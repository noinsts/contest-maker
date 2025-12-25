#include "app/SettingsWindow.hpp"

#include <core/ConfigManager.hpp>
#include <app/EditTemplateDialog.hpp>

SettingsWindow::SettingsWindow() {
	loadLanguages();
	setupUI();
}

void SettingsWindow::loadLanguages() {
	languages = ConfigManager::loadConfig();
	originalLanguages = languages;
}

void SettingsWindow::setupUI() {
	set_title("Налаштування шаблонів");
	set_default_size(WINDOW_WIDTH, WIDTH_HEIGHT);

	scrolled.add(listbox);
	scrolled.set_min_content_height(300);
	scrolled.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	vbox.pack_start(scrolled, Gtk::PACK_EXPAND_WIDGET);

	buttonsBox.set_spacing(10);
	vbox.pack_start(buttonsBox, Gtk::PACK_SHRINK);

	buttonsBox.pack_end(cancelButton, Gtk::PACK_SHRINK);
	cancelButton.get_style_context()->add_class("destructive-action");
	cancelButton.signal_clicked().connect(
		sigc::mem_fun(*this, &SettingsWindow::onCancelButtonClick)
	);

	buttonsBox.pack_end(applyButton, Gtk::PACK_SHRINK);
	applyButton.get_style_context()->add_class("suggested-button");
	applyButton.signal_clicked().connect(
		sigc::mem_fun(*this, &SettingsWindow::onApplyButtonClick)
	);

	for (size_t i = 0; i < languages.size(); i++) {
		renderLanguageRow(i);
	}

	vbox.set_spacing(10);
	vbox.set_margin_top(10);
	vbox.set_margin_bottom(10);
	vbox.set_margin_left(10);
	vbox.set_margin_right(10);

	add(vbox);
	show_all();
}

void SettingsWindow::renderLanguageRow(size_t index) {
	auto& lang = languages[index];

	auto* rowBox = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL);
	rowBox->set_spacing(10);
	rowBox->set_margin_top(5);
	rowBox->set_margin_bottom(5);

	auto* toggle = Gtk::make_managed<Gtk::Switch>();
	toggle->set_active(lang.enabled);
	toggle->signal_state_set().connect(
		[&lang](bool state) {
			lang.enabled = state;
			return false;
		}
	);

	auto* titleLabel = Gtk::make_managed<Gtk::Label>();
	titleLabel->set_xalign(0);
	titleLabel->set_markup("<b>" + lang.name + "</b>");

	auto* editButton = Gtk::make_managed<Gtk::Button>("Редагувати");
	editButton->set_relief(Gtk::RELIEF_NONE);
	editButton->get_style_context()->add_class("suggested-button");
	editButton->signal_clicked().connect([this, index] {
		onEditButtonClick(index);
	});

	rowBox->pack_start(*titleLabel, Gtk::PACK_SHRINK);
	rowBox->pack_end(*toggle, Gtk::PACK_SHRINK);
	rowBox->pack_end(*editButton, Gtk::PACK_SHRINK);

	auto* row = Gtk::make_managed<Gtk::ListBoxRow>();
	row->add(*rowBox);
	listbox.add(*row);
}

void SettingsWindow::onEditButtonClick(size_t index) {
	auto& lang = languages[index];
	EditTemplateDialog dialog(*this, lang.extension, lang.getActiveTemplate());
	int result = dialog.run();
	if (result == Gtk::RESPONSE_OK) {
		std::string newTemplate = dialog.getTemplate();
		if (newTemplate != lang.defaultTemplate) {
			lang.customTemplate = newTemplate;
		} else {
			lang.customTemplate.clear();
		}
	}
}

void SettingsWindow::onApplyButtonClick() {
	try {
		ConfigManager::saveConfig(languages);
		hide();
	}
	catch (const std::exception& e) {
		Gtk::MessageDialog dialog(
			*this,
			"Помилка",
			false,
			Gtk::MESSAGE_ERROR,
			Gtk::BUTTONS_OK,
			true
		);
		dialog.set_secondary_text("Не вдалося зберегти налаштування");
		dialog.run();
	}
}

void SettingsWindow::onCancelButtonClick() {
	if (hasChange() && !showConfirmationDialog(
			"Підтвердження",
			"Закрити вікно без збереження?"
	)) return;
	hide();
}

bool SettingsWindow::showConfirmationDialog(const Glib::ustring& title, const Glib::ustring& text) {
	Gtk::MessageDialog dialog(
		*this,
		title,
		false,
		Gtk::MESSAGE_QUESTION,
		Gtk::BUTTONS_OK_CANCEL,
		true
	);
	dialog.set_secondary_text(text);
	return dialog.run() == Gtk::RESPONSE_OK;
}

bool SettingsWindow::hasChange() {
	return false;
}