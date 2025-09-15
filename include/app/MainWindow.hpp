#pragma once

#include <gtkmm.h>
#include <string>

/**
 * @class MainWindow
 * @brief Головне вікно застосунку
 */
class MainWindow : public Gtk::Window {

public:
    /**
     * @brief Конструктор головного вікна.
     * Ініціалізує UI та підключає сигнали
     */
    MainWindow();

private:
    static constexpr int WINDOW_WIDTH = 250; ///< Ширина вікна у пікселях
    static constexpr int WINDOW_HEIGHT = 100; ///< Висота вікна у пікселях
    static constexpr int DEFAULT_COMBO_SELECTION = 4; ///< E (5 задач) - дефолтний вибір

    // Layout
    Gtk::Box vbox{ Gtk::ORIENTATION_VERTICAL };

    // Contest name
    Gtk::Label contestNameLabel{ "Вкажіть назву контесту:" };
    Gtk::Entry contestNameEntry;

    // Problem count
    Gtk::Label problemCountLabel{ "Оберіть кількість задач:" };
    Gtk::ComboBoxText problemCountSelector;

    // Directory path
    Gtk::Label directoryLabel{ "Виберіть папку:" };
    Gtk::Button directoryPicker{ "Обрати папку" };
    std::string folderName;

    // Checkboxes
    Gtk::CheckButton openInVSCodeOption{ "Відкрити в Code" };
    Gtk::CheckButton initGitRepoOption{ "Створити Git репозиторій" };

    // Create
    Gtk::Button createButton{ "Створити" };

    /**
    * @brief Налаштовує UI-компоненти та додає їх у головний контейнер.
    */
    void setupUI();

    /**
     * @brief Підключає сигнали кнопок до відповідних обробників.
     */
    void connectSignals();

    /**
     * @brief Ініціалізує селектор кількість задач
     */
    void initializeProblemCountSelector();

    /**
     * @brief Викликає файловий діалог для вибору папки.
     * Після вибору - зберігає їх шлях у 'folderName' і оновлює текст кнопки.
     */
    void onFolderButtonPress();

    /**
     * @brief Обробник кнопки "Створити".
     */
    void onCreateButtonPress();

    /**
     * @brief Перевіряє коректність введених даних.
     * @throws invalid_argument Якщо назва або шлях не коректні.
     */
    void validateFormInputs(const std::string& contestName) const;

    /**
     * @brief Очищує поля до їх дефолтних значень.
     */
    void resetForm();

    /**
     * @brief Показує діалог помилки.
     * @param title Заголовок діалогу.
     * @param subTitle Пояснення помилки.
     */
    void showErrorDialog(const std::string& title, const std::string& subTitle);

    /**
    * @brief Показує діалог успіху.
    * @param title Заголовок діалогу.
    * @param subTitle Пояснення помилки.
    */
    void showSuccessDialog(const std::string& title, const std::string& subTitle);
};
