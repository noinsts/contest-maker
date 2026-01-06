#pragma once

#include <gtkmm.h>
#include <vector>
#include <core/LanguageTemplate.hpp>

/**
 * @class SettingsWindow.
 * @brief Вікно налаштувань для керування шаблонами мов програмування.
 */
class SettingsWindow : public Gtk::Window {
public:
	/**
	 * @brief Конструктор вікна налаштувань.
	 * Ініціалізуй компоненти UI та завантажує дані з конфігурації.
	 */
	SettingsWindow();

private:
	static constexpr int WINDOW_WIDTH = 400; ///< Дефолтна ширина вікна.
	static constexpr int WINDOW_HEIGHT = 400; ///< Дефолтна висота вікна.

	// UI компоненти
	Gtk::Box vbox{ Gtk::ORIENTATION_VERTICAL }; ///< Головний вертикальний контейнер.
	Gtk::ListBox listbox; ///< Список рядків з мовами.
	Gtk::Box buttonsBox{ Gtk::ORIENTATION_HORIZONTAL }; /// Контейнер для кнопок дій.
	Gtk::ScrolledWindow scrolled; ///< Область прокрутки для списку мов.

	Gtk::Button applyButton{ "Apply" }; ///< Кнопка збереження змін.
	Gtk::Button cancelButton{ "Cancel" }; ///< Кнопка скасування/закриття.

	// Дані
	std::vector<LanguageTemplate> languages; ///< Поточний стан шаблонів.
	std::vector<LanguageTemplate> originalLanguages; ///< Початковий стан шаблонів.

	/**
	 * @brief Обробник натискання кнопки "Apply".
	 * Зберігає поточні налаштування через ConfigManager.
	 */
	void onApplyButtonClick();

	/**
	 * @brief Обробник натискання кнопки "Cancel".
	 * Закриває вікно, перевіряючи наявність незбережених змін.
	 */
	void onCancelButtonClick();

	/**
	 * @brief Обробник натискання кнопки редагування.
	 * @param index Індекс мови у вікторі @ref languages.
	 */
	void onEditButtonClick(size_t index);

	/**
	 * @brief Перевіряє, чи будуть внесені зміни порівняно з оригіналом.
	 * @return true, якщо дані змінено, інакше false.
	 */
	bool hasChange() const;

	/**
	 * @brief Відображає модальне вікно підтвердження.
	 * @param title Заголовок діалогу.
	 * @param text Текст запитання.
	 * @return true, якщо користувач натиснув OK.
	 */
	bool showConfirmationDialog(const Glib::ustring& title, const Glib::ustring& text);

	/**
	 * @brief Створює та додає віджет рядка для конкретної мови в listbox.
	 * @param index Індекс мови у вікторі @ref languages.
	 */
	void renderLanguageRow(size_t index);

	/**
	 * @brief Налаштовує UI компоненти та позіціонує їх.
	 */
	void setupUI();

	/**
	 * @brief Завантажує дані мов з ConfigManager.
	 */
	void loadLanguages();
};
