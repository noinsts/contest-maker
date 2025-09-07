#include <gtkmm.h>

#include <iostream>
#include <fstream>
#include <filesystem>

#include <TemplateManager.hpp>

namespace fs = std::filesystem;
using namespace std;


class ContentBuilder {

private:
    string directoryName;
    char maxSubDir;
    fs::path basePath;

    fs::path fullDirPath() const {
        return basePath / directoryName;
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
            file.close();
        
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
            TemplateManager::getCMakeTemplate(directoryName, maxSubDir)
        );

        // README
        createFile(
            fullDirPath() / "README.md",
            TemplateManager::getReadmeTemplate(directoryName)
        );
    }

    // ==============================
    // ==============================

    void createSubDirectories() {
        for (char c = 'A'; c <= maxSubDir; ++c) {
            fs::path dirPath = fullDirPath() / string{c};
            createDirectory(dirPath);
        }
    }

    void createSourceFiles() const {
        for (char c = 'A'; c <= maxSubDir; c++) {
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
        : directoryName(std::move(contestName)), maxSubDir(maxDir), basePath(std::move(base)) {}

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
    Gtk::Box vbox{Gtk::ORIENTATION_VERTICAL};
    Gtk::Label labelName{"Вкажіть назву контесту:"};
    Gtk::Entry nameEntry;

    Gtk::Label labelMaxProblem{"Оберіть кількість задач:"};
    Gtk::ComboBoxText maxContestProblem;

    Gtk::Label labelFolder{"Виберіть папку:"};
    Gtk::Button folder{"Обрати папку"};

    Gtk::CheckButton openInCodeCheck{"Відкрити в Code"};

    Gtk::Button createButton{"Створити"};

    string folderName;

    void setupUI() {
        set_default_size(250, 100);
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
        maxContestProblem.set_active(4);
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

        const string forbiddenChars = "<>:\"/\\|?*";
        for (char c : contestName) {
            if (forbiddenChars.find(c) != string::npos) {
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
