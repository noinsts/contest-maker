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
            if (!fs::create_directory(path)) {
                cerr << "Directory already exists: " << path << endl;
            }
        } catch (const fs::filesystem_error& e) {
            throw runtime_error("Failed to create directory: " + path.string() + " - " + e.what());
        }
    }

    void createFile(const fs::path& filePath, const string& context) const {
        ofstream file(filePath);
        if (!file.is_open()) {
            throw runtime_error("Failed to create file: " + filePath.string());
        }
        file << context;
        file.close();
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

        set_default_size(250, 100);
        set_title("Contest maker");

        vbox.pack_start(label, Gtk::PACK_SHRINK);
        vbox.pack_start(nameEntry, Gtk::PACK_SHRINK);

        vbox.pack_start(label2, Gtk::PACK_SHRINK);

        for (char c = 'A'; c <= 'Z'; c++) {
            maxContestProblem.append(string{c});
        }

        vbox.pack_start(maxContestProblem, Gtk::PACK_SHRINK);

        vbox.set_spacing(10);
        vbox.pack_start(label3, Gtk::PACK_SHRINK);

        vbox.pack_start(folder, Gtk::PACK_SHRINK);

        vbox.pack_start(create, Gtk::PACK_SHRINK);

        add(vbox);
        show_all_children();
    }

private:
    Gtk::Box vbox{Gtk::ORIENTATION_VERTICAL};
    Gtk::Label label{"Enter a contest name"};
    Gtk::Entry nameEntry;

    Gtk::Label label2{"Select max contest problem"};
    Gtk::ComboBoxText maxContestProblem;

    Gtk::Label label3{"Select a folder pos"};
    Gtk::Button folder{"Відкрити"};

    Gtk::Button create{"Create"};

    string folderName;

    void connectSignals() {
        create.signal_clicked().connect(
            sigc::mem_fun(*this, &MainWindow::onCreateButtonPress)
        );
        folder.signal_clicked().connect(
            sigc::mem_fun(*this, &MainWindow::onFolderButtonPress)
        );
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
        const string contestName = nameEntry.get_text();

        if (contestName.empty()) {
            generateError("Невірно введені дані", "Ви не вказали назву контесту");
            return;
        }

        string maxDir = maxContestProblem.get_active_text();

        if (maxDir.empty()) {
            generateError("Невірно введені дані", "Ви не вказали кількість задач");
            return;
        }

        if (folderName.empty()) {
            generateError("Невірно введені дані", "Ви не вказали шлях до директорії");
            return;
        }

        try {
            ContentBuilder builder(contestName, maxDir[0], fs::path(folderName));
            builder.run();

            generateInfo("Успіх!", "Папку створено.");

        } catch (const exception& e) {
            generateError("Помилка", e.what());
        }

    }

    void generateError(const string& title, const string& subTitle) {
        Gtk::MessageDialog dialog(*this, "Помилка", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
        dialog.set_title(title);
        dialog.set_secondary_text(subTitle);
        dialog.run();
    }

    void generateInfo(const string& title, const string& subTitle) {
        Gtk::MessageDialog dialog(*this, "Інфо", false, Gtk::MESSAGE_INFO);
        dialog.set_title(title);
        dialog.set_secondary_text(subTitle);
        dialog.run();
    }

};

int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create(argc, argv, "com.noinsts.contest-maker");
    MainWindow window;
    return app -> run(window);
}
