#include <gtkmm.h>

#include <iostream>
#include <filesystem>

#include <TemplateManager.hpp>
#include <SystemHelper.hpp>
#include <FileSystemManager.hpp>

namespace fs = std::filesystem;
using namespace std;

class ContentBuilder {

private:
    string directoryName_;
    char maxSubDir_;
    fs::path basePath_;

    [[nodiscard]] fs::path fullDirPath() const {
        return basePath_ / directoryName_;
    }

    // ==============================
    // ==============================

    void createMainDirectory() {
        FileSystemManager::createDirectory(fullDirPath());
    }

    void createMainFiles() {
        // CMake
        FileSystemManager::createFile(
            fullDirPath() / "CMakeLists.txt",
            TemplateManager::getCMakeTemplate(directoryName_, maxSubDir_)
        );

        // README
        FileSystemManager::createFile(
            fullDirPath() / "README.md",
            TemplateManager::getReadmeTemplate(directoryName_)
        );
    }

    // ==============================
    // ==============================

    void createSubDirectories() {
        for (char c = 'A'; c <= maxSubDir_; ++c) {
            const fs::path dirPath = fullDirPath() / string{c};
            FileSystemManager::createDirectory(dirPath);
        }
    }

    void createSourceFiles() const {
        for (char c = 'A'; c <= maxSubDir_; c++) {
            const fs::path subDir = fullDirPath() / string{c};
            const string fileName = string{c};

            // C++ файли
            FileSystemManager::createFile(
                subDir / (fileName + ".cpp"),
                TemplateManager::getCppTemplate()
            );

            // Python файли
            FileSystemManager::createFile(
                subDir / (fileName + ".py"),
                TemplateManager::getPythonTemplate()
            );
        }
    }

public:
    ContentBuilder(string contestName, char maxDir, fs::path base)
        : directoryName_(std::move(contestName)), maxSubDir_(maxDir), basePath_(std::move(base)) {}

    void run() {
        createMainDirectory();
        createMainFiles();

        createSubDirectories();
        createSourceFiles();
    }
};


class MainWindow : public Gtk::Window {

public: 
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

    void onCreateButtonPress() {
        try {
            const string contestName = contestNameEntry.get_text();
            const string maxDir = problemCountSelector.get_active_text();

            validateFormInputs(contestName);

            ContentBuilder builder(contestName, maxDir[0], fs::path(folderName));
            builder.run();

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

        } catch (const exception& e) {
            showErrorDialog("Помилка", e.what());
        }

    }

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

    void showErrorDialog(const string& title, const string& subTitle) {
        Gtk::MessageDialog dialog(*this, title, false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
        dialog.set_secondary_text(subTitle);
        dialog.run();
    }

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
