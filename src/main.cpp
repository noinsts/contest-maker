#include <gtkmm.h>

#include <iostream>
#include <filesystem>

#include <TemplateManager.hpp>
#include <SystemHelper.hpp>
#include <FileSystemManager.hpp>
#include <ContestStructureBuilder.hpp>

namespace fs = std::filesystem;
using namespace std;

class MainWindow : public Gtk::Window {

public: 
    /**
     * @brief Конструктор головного вікна.
     * Ініціалізує UI та підключає сигнали
     */
    MainWindow() {
        connectSignals();
        setupUI();
    }

private:
    static constexpr int WINDOW_WIDTH = 250;
    static constexpr int WINDOW_HEIGHT = 100;
    static constexpr int DEFAULT_COMBO_SELECTION = 4; // E (5 problems)

    // Layout
    Gtk::Box vbox{Gtk::ORIENTATION_VERTICAL};

    // Contest name
    Gtk::Label contestNameLabel{"Вкажіть назву контесту:"};
    Gtk::Entry contestNameEntry;

    // Problem count
    Gtk::Label problemCountLabel{"Оберіть кількість задач:"};
    Gtk::ComboBoxText problemCountSelector;

    // Directory path
    Gtk::Label directoryLabel{"Виберіть папку:"};
    Gtk::Button directoryPicker{"Обрати папку"};
    string folderName;

    // Checkboxes
    Gtk::CheckButton openInVSCodeOption{"Відкрити в Code"};
    Gtk::CheckButton initGitRepoOption{"Створити Git репозиторій"};

    // Create
    Gtk::Button createButton{"Створити"};

    /**
     * @brief Налаштовує UI-компоненти та додає їх у головний контейнер.
     */
    void setupUI() {
        set_default_size(WINDOW_WIDTH, WINDOW_HEIGHT);
        set_title("Contest maker");
        set_position(Gtk::WIN_POS_CENTER);

        vbox.set_spacing(10);
        vbox.set_margin_top(10);
        vbox.set_margin_bottom(10);
        vbox.set_margin_left(10);
        vbox.set_margin_right(10);

        // Contest name
        vbox.pack_start(contestNameLabel, Gtk::PACK_SHRINK);
        vbox.pack_start(contestNameEntry, Gtk::PACK_SHRINK);

        // Problem count
        vbox.pack_start(problemCountLabel, Gtk::PACK_SHRINK);
        initializeProblemCountSelector();
        vbox.pack_start(problemCountSelector, Gtk::PACK_SHRINK);

        // Directory path
        vbox.pack_start(directoryLabel, Gtk::PACK_SHRINK);
        vbox.pack_start(directoryPicker, Gtk::PACK_SHRINK);

        // Checkboxes
        vbox.pack_start(openInVSCodeOption, Gtk::PACK_SHRINK);
        vbox.pack_start(initGitRepoOption, Gtk::PACK_SHRINK);

        // Create
        vbox.pack_start(createButton, Gtk::PACK_SHRINK);

        add(vbox);
        show_all_children();
    }

    /**
     * @brief Підключає сигнали кнопок до відповідних обробників.
     */
    void connectSignals() {
        createButton.signal_clicked().connect(
            sigc::mem_fun(*this, &MainWindow::onCreateButtonPress)
        );
        directoryPicker.signal_clicked().connect(
            sigc::mem_fun(*this, &MainWindow::onFolderButtonPress)
        );
    }

    /**
     * @brief Ініціалізує селектор кількість задач
     */
    void initializeProblemCountSelector() {
        for (char c = 'A'; c <= 'Z'; c++) {
            problemCountSelector.append(string{c});
        }
        problemCountSelector.set_active(DEFAULT_COMBO_SELECTION);
    }

    /**
     * @brief Викликає файловий діалог для вибору папки.
     * Після вибору - зберігає їх шлях у 'folderName' і оновлює текст кнопки.
     */
    void onFolderButtonPress() {
        auto dialog = Gtk::FileChooserNative::create(
            "Оберіть директорію",
            *this,
            Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER,
            "_Select",
            "_Cancel"
        );

        if (dialog -> run() == Gtk::RESPONSE_ACCEPT) {
            folderName = dialog -> get_filename();
            directoryPicker.set_label("🗂 " + fs::path(folderName).filename().string());
        }
    }

    /**
     * @brief Обробник кнопки "Створити".
     */
    void onCreateButtonPress() {
        try {
            const string contestName = contestNameEntry.get_text();
            const string maxDir = problemCountSelector.get_active_text();

            validateFormInputs(contestName);

            ContestStructureBuilder builder(contestName, maxDir[0], fs::path(folderName));
            builder.build();

            showSuccessDialog("Успіх!", "Папку створено.");

            if (openInVSCodeOption.get_active()) {
                const fs::path fullPath = fs::path(folderName) / contestName;
                SystemHelper::openInCode(fullPath.string());
            }

            if (initGitRepoOption.get_active()) {
                const fs::path fullPath = fs::path(folderName) / contestName;
                SystemHelper::createGitRepo(fullPath.string());
            }

            resetForm();
        } 
        catch (const exception& e) {
            showErrorDialog("Помилка", e.what());
        }

    }

    /**
     * @brief Перевіряє коректність введених даних.
     * @throws invalid_argument Якщо назва або шлях не коректні.
     */
    void validateFormInputs(const string& contestName) const {
        if (contestName.empty()) {
            throw invalid_argument("Ви не вказали назву контесту");
        }

        if (folderName.empty()) {
            throw invalid_argument("Ви не вказали шлях до директорії");
        }

        static const string FORBIDDEN_CHARS = "<>:\"/\\|?*";
        for (char c : contestName) {
            if (FORBIDDEN_CHARS.find(c) != string::npos) {
                throw invalid_argument("Назва контесту містить заборонений символ: " + c);
            }
        }
    }

    /**
     * @brief Очищує поля до їх дефолтних значень.
     */
    void resetForm() {
        contestNameEntry.set_text("");
        folderName.clear();
        problemCountSelector.set_active(DEFAULT_COMBO_SELECTION);
        directoryPicker.set_label("Обрати папку");
        openInVSCodeOption.set_active(false);
        initGitRepoOption.set_active(false);
    }

    /**
     * @brief Показує діалог помилки.
     * @param title Заголовок діалогу.
     * @param subTitle Пояснення помилки.
     */
    void showErrorDialog(const string& title, const string& subTitle) {
        Gtk::MessageDialog dialog(*this, title, false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
        dialog.set_secondary_text(subTitle);
        dialog.run();
    }

    /**
     * @brief Показує діалог успіху.
     * @param title Заголовок діалогу.
     * @param subTitle Пояснення помилки.
     */
    void showSuccessDialog(const string& title, const string& subTitle) {
        Gtk::MessageDialog dialog(*this, title, false, Gtk::MESSAGE_INFO);
        dialog.set_secondary_text(subTitle);
        dialog.run();
    }

};

int main(int argc, char *argv[]) {
    try {
        auto app = Gtk::Application::create(argc, argv, "com.noinsts.contest-maker");
        MainWindow window;
        return app -> run(window);
    } 
    catch (const exception& e) {
        cerr << "Application error: " << e.what() << endl;
        return 1;
    }
}
