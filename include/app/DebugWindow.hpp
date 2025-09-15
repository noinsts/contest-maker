#pragma once

#include <gtkmm.h>

/**
 * @class DebugWindow
 * @brief Дебаг вікно для відкладки
 */
class DebugWindow : public Gtk::Window {

public:
    /**
     * @brief Конструктор класу DebugWindow
     */
    DebugWindow();

    /**
     * @brief Додає до textView текст.
     * @param text Текст, що треба додати.
     */
    void addText(const std::string& text);

private:
    static constexpr int WINDOW_WIDTH = 600; ///< Ширина вікна у пікселях
    static constexpr int WINDOW_HEIGHT = 400; ///< Висота вікна у пікселях

    Gtk::TextView textView;
    Gtk::ScrolledWindow scrolled;

    /**
     * @brief Встановлює та налаштовує UI-компоненти.
     */
    void setupUI();
};
