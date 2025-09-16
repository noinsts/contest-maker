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

private:
    static constexpr int WINDOW_WIDTH = 800; ///< Ширина вікна у пікселях
    static constexpr int WINDOW_HEIGHT = 600; ///< Висота вікна у пікселях

    Gtk::TextView textView;
    Gtk::ScrolledWindow scrolled;

    /**
     * @brief Встановлює та налаштовує UI-компоненти.
     */
    void setupUI();
    
    /**
     * @brief Оновлює текст в textView.
     */
    bool updateTextView();

    /**
     * @brief Додає до textView текст.
     */
    void addText(const std::string& text);
};
