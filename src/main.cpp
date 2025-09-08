#include <gtkmm.h>

#include <iostream>
#include <fstream>
#include <filesystem>

#include <TemplateManager.hpp>

namespace fs = std::filesystem;
using namespace std;

/**
 * @class SystemHelper
 * @brief Клас з допоміжних методів керування системою
 */
class SystemHelper {

public:
    /**
     * @brief Метод відкриває директорію в Visual Studio Code
     * @param fullPath шлях до директорії
     */
    static void openInCode(const string& fullPath) {
        string command;

        #ifdef _WIN32
            command = "start \"\" \"%LOCALAPPDATA%\\Programs\\Microsoft VS Code\\Code.exe\" \"" + fullPath + "\"";
        #elif __APPLE__
            command = "open -a \"Visual Studio Code\" \"" + fullPath + "\"";
        #else
            command = "code \"" + fullPath + "\" &";
        #endif

        int result = system(command.c_str());

        if (result != 0) {
            throw runtime_error("Не вдалося відкрити Code");
        }
    }
    
    /**
     * @brief Метод створює Git репозиторій
     * @param fullPath шлях до директорії
     */
    static void createGitRepo(const string& fullPath) {
        string escapedPath = "\"" + fullPath + "\"";
        const string command = "cd " + escapedPath + " && git init";

        int result = system(command.c_str());

        if (result != 0) {
            throw runtime_error("Помилка під час створення Git репозиторію");
        }
    }

};


class ContentBuilder {

private:
    string directoryName_;
    char maxSubDir_;
    fs::path basePath_;

    [[nodiscard]] fs::path fullDirPath() const {
        return basePath_ / directoryName_;
    }

    void createDirectory(const fs::path& path) {
        try {
            if (fs::exists(path)) {
                cout << "Directory already exists, skipping: " << path << endl;
                return;
            }

            if (fs::create_directories(path)) {
                cout << "Created directory: " << path << endl;
            }
        } catch (const fs::filesystem_error& e) {
            throw runtime_error("Failed to create directory: " + path.string() + " - " + e.what());
        }
    }

    void createFile(const fs::path& filePath, const string& context) const {
        try {
            if (fs::exists(filePath)) {
                cout << "File altready exists, skipping: " << filePath.string() << endl;
                return;
            }

            ofstream file(filePath);
            if (!file.is_open()) {
                throw runtime_error("Cannot open file for writting" + filePath.string());
            }

            file << context;
            if (file.fail()) {
                throw runtime_error("Error writing to file: " + filePath.string());
            }

            cout << "Created file: " << filePath.string() << endl;
        
        } catch (const exception& e) {
            throw runtime_error("Failed to create file: " + filePath.string());
        } 
    }

    // ==============================
    // ==============================

    void createMainDirectory() {
        createDirectory(fullDirPath());
    }

    void createMainFiles() {
        // CMake
        createFile(
            fullDirPath() / "CMakeLists.txt",
            TemplateManager::getCMakeTemplate(directoryName_, maxSubDir_)
        );

        // README
        createFile(
            fullDirPath() / "README.md",
            TemplateManager::getReadmeTemplate(directoryName_)
        );
    }

    // ==============================
    // ==============================

    void createSubDirectories() {
        for (char c = 'A'; c <= maxSubDir_; ++c) {
            const fs::path dirPath = fullDirPath() / string{c};
            createDirectory(dirPath);
        }
    }

    void createSourceFiles() const {
        for (char c = 'A'; c <= maxSubDir_; c++) {
            const fs::path subDir = fullDirPath() / string{c};
            const string fileName = string{c};

            // C++ файли
            createFile(
                subDir / (fileName + ".cpp"),
                TemplateManager::getCppTemplate()
            );

            // Python файли
            createFile(
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
    static constexpr int DEFAULT_COMBO_SELECTION = 4;

    Gtk::Box vbox{Gtk::ORIENTATION_VERTICAL};
    Gtk::Label labelName{"Вкажіть назву контесту:"};
    Gtk::Entry nameEntry;

    Gtk::Label labelMaxProblem{"Оберіть кількість задач:"};
    Gtk::ComboBoxText maxContestProblem;

    Gtk::Label labelFolder{"Виберіть папку:"};
    Gtk::Button folder{"Обрати папку"};

    Gtk::CheckButton openInCodeCheck{"Відкрити в Code"};
    Gtk::CheckButton createGitRepoCheck{"Створити Git репозиторій"};

    Gtk::Button createButton{"Створити"};

    string folderName;

    void setupUI() {
        set_default_size(WINDOW_WIDTH, WINDOW_HEIGHT);
        set_title("Contest maker");
        set_position(Gtk::WIN_POS_CENTER);

        vbox.set_spacing(10);
        vbox.set_margin_top(10);
        vbox.set_margin_bottom(10);
        vbox.set_margin_left(10);
        vbox.set_margin_right(10);

        vbox.pack_start(labelName, Gtk::PACK_SHRINK);
        vbox.pack_start(nameEntry, Gtk::PACK_SHRINK);

        vbox.pack_start(labelMaxProblem, Gtk::PACK_SHRINK);

        populateComboBox();
        vbox.pack_start(maxContestProblem, Gtk::PACK_SHRINK);

        vbox.set_spacing(10);
        vbox.pack_start(labelFolder, Gtk::PACK_SHRINK);

        vbox.pack_start(folder, Gtk::PACK_SHRINK);

        vbox.pack_start(openInCodeCheck, Gtk::PACK_SHRINK);
        vbox.pack_start(createGitRepoCheck, Gtk::PACK_SHRINK);

        vbox.pack_start(createButton, Gtk::PACK_SHRINK);

        add(vbox);
        show_all_children();
    }

    void connectSignals() {
        createButton.signal_clicked().connect(
            sigc::mem_fun(*this, &MainWindow::onCreateButtonPress)
        );
        folder.signal_clicked().connect(
            sigc::mem_fun(*this, &MainWindow::onFolderButtonPress)
        );
    }

    void populateComboBox() {
        for (char c = 'A'; c <= 'Z'; c++) {
            maxContestProblem.append(string{c});
        }
        maxContestProblem.set_active(DEFAULT_COMBO_SELECTION);
    }

    void onFolderButtonPress() {
        auto dialog = Gtk::FileChooserNative::create(
            "Оберіть директорію",
            *this,
            Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER,
            "_Select",
            "_Cancel"
        );

        int result = dialog -> run();

        if (result == Gtk::RESPONSE_ACCEPT) {
            folderName = dialog -> get_filename();
            folder.set_label("🗂 " + fs::path(folderName).filename().string());
        }
    }

    void onCreateButtonPress() {
        try {
            const string contestName = nameEntry.get_text();
            const string maxDir = maxContestProblem.get_active_text();

            validateFormInputs(contestName);

            ContentBuilder builder(contestName, maxDir[0], fs::path(folderName));
            builder.run();

            showSuccessDialog("Успіх!", "Папку створено.");

            if (openInCodeCheck.get_active()) {
                const fs::path fullPath = fs::path(folderName) / contestName;
                SystemHelper::openInCode(fullPath.string());
            }

            if (createGitRepoCheck.get_active()) {
                const fs::path fullPath = fs::path(folderName) / contestName;
                SystemHelper::createGitRepo(fullPath.string());
            }

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

    void showErrorDialog(const string& title, const string& subTitle) {
        Gtk::MessageDialog dialog(*this, "Помилка", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
        dialog.set_title(title);
        dialog.set_secondary_text(subTitle);
        dialog.run();
    }

    void showSuccessDialog(const string& title, const string& subTitle) {
        Gtk::MessageDialog dialog(*this, "Інфо", false, Gtk::MESSAGE_INFO);
        dialog.set_title(title);
        dialog.set_secondary_text(subTitle);
        dialog.run();
    }

};

int main(int argc, char *argv[]) {
    try {
        auto app = Gtk::Application::create(argc, argv, "com.noinsts.contest-maker");
        MainWindow window;
        return app -> run(window);
    } catch (const exception& e) {
        cerr << "Application error: " << e.what() << endl;
        return 1;
    }
}
