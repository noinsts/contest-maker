#include "app/SettingsWindow.hpp"

#include <core/ConfigManager.hpp>

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

	scrolled.add(languagesBox);
	scrolled.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	vbox.pack_start(scrolled, Gtk::PACK_EXPAND_WIDGET);
	vbox.pack_start(buttonsBox, Gtk::PACK_EXPAND_WIDGET);

	buttonsBox.pack_start(applyButton, Gtk::PACK_SHRINK);
	applyButton.signal_clicked().connect(
		sigc::mem_fun(*this, &SettingsWindow::onApplyButtonClick)
	);

	buttonsBox.pack_start(cancelButton, Gtk::PACK_SHRINK);
	cancelButton.signal_clicked().connect(
		sigc::mem_fun(*this, &SettingsWindow::onCancelButtonClick)
	);

	for (size_t i = 0; i < languages.size(); i++) {
		renderLanguageRow(i);
	}

	add(vbox);
	show_all();
}

void SettingsWindow::renderLanguageRow(size_t index) {
	auto& lang = languages[index];

	auto* rowBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL));
	rowBox->set_spacing(10);
	rowBox->set_margin_top(5);
	rowBox->set_margin_bottom(5);

	auto* checkbox = Gtk::manage(new Gtk::CheckButton());
	checkbox->set_active(lang.enabled);
	checkbox->signal_toggled().connect([this, index] {
		languages[index].enabled = !languages[index].enabled;
	});

	auto* titleLabel = Gtk::manage(new Gtk::Label(lang.name));

	rowBox->pack_start(*checkbox, Gtk::PACK_SHRINK);
	rowBox->pack_start(*titleLabel, Gtk::PACK_SHRINK);

	languagesBox.pack_start(*rowBox, Gtk::PACK_SHRINK);
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
	return false; // TODO: implement logic
}