#pragma once

#include <string>
#include <gtkmm.h>
#include <gtksourceviewmm.h>

/**
 * @class EditTemplateDialog.
 * @brief Діалогове вікно для редагування шаблону
 * 
 * Використовується для відображення та редагування текстового шаблону
 * з підсвіткою синтаксису (GtkSourceView).
 * Повертає оновлений код після закриття діалогу.
 */
class EditTemplateDialog : public Gtk::Dialog {
public:
	/**
	 * @brief Контруктор діалогу редагування шаблону.
	 * @param parent Батьківське вікно (для модального режиму).
	 * @param languageName Назва мови програмування (для заголовка та підсвітки).
	 * @param templateCode Початковий шаблон коду.
	 */
	EditTemplateDialog(
		Gtk::Window& parent, 
		const std::string& languageName, 
		const std::string& templateCode
	);

	/**
	 * @brief Деконструктор класу.
	 */
	virtual ~EditTemplateDialog() = default;

	/**
	 * @brief Отримати відредагований шаблон.
	 * @return Рядок з кодом шаблону після редагування.
	 */
	std::string getTemplate();

private:
	static constexpr int WINDOW_WIDTH = 600; ///< Ширина вікна за замовченням.
	static constexpr int WINDOW_HEIGHT = 500; ///< Висота вікна за замовченням.

	Gtk::Box vbox{ Gtk::ORIENTATION_VERTICAL }; ///< Основний вектикальний контейнер.
	Gtk::ScrolledWindow scrolled; ///< Scroll-контейнер для редагування коду.

	Gtk::Label titleLabel; ///< Заголовок діалогу.

	Gsv::View sourceView; ///< GtkSourceView для редагування шаблону.
	Glib::RefPtr<Gsv::Buffer> sourceBuffer; ///< Буфер тексту шаблону.

	/**
	 * @brief Налаштовує UI діалогу.
	 * 
	 * Створює віджети, налаштовує розміри, підсвітку синтаксису
	 * та додає усе в контейнер.
	 */
	void setupUI();
};