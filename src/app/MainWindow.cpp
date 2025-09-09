#include "app/MainWindow.hpp"

#include <string>
#include <filesystem>
#include <stdexcept>

#include <core/SystemHelper.hpp>
#include <core/ContestStructureBuilder.hpp>

MainWindow::MainWindow() {
	connectSignals();
	setupUI();
}

void MainWindow::setupUI() {
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

void MainWindow::connectSignals() {
    createButton.signal_clicked().connect(
        sigc::mem_fun(*this, &MainWindow::onCreateButtonPress)
    );
    directoryPicker.signal_clicked().connect(
        sigc::mem_fun(*this, &MainWindow::onFolderButtonPress)
    );
}

void MainWindow::initializeProblemCountSelector() {
    for (char c = 'A'; c <= 'Z'; c++) {
        problemCountSelector.append(std::string{ c });
    }
    problemCountSelector.set_active(DEFAULT_COMBO_SELECTION);
}

void MainWindow::onFolderButtonPress() {
    auto dialog = Gtk::FileChooserNative::create(
        "Оберіть директорію",
        *this,
        Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER,
        "_Select",
        "_Cancel"
    );

    if (dialog->run() == Gtk::RESPONSE_ACCEPT) {
        folderName = dialog->get_filename();
        directoryPicker.set_label("🗂 " + std::filesystem::path(folderName).filename().string());
    }
}

void MainWindow::onCreateButtonPress() {
    try {
        const std::string contestName = contestNameEntry.get_text();
        const std::string maxDir = problemCountSelector.get_active_text();

        validateFormInputs(contestName);

        ContestStructureBuilder builder(contestName, maxDir[0], std::filesystem::path(folderName));
        builder.build();

        showSuccessDialog("Успіх!", "Папку створено.");

        if (openInVSCodeOption.get_active()) {
            const std::filesystem::path fullPath = std::filesystem::path(folderName) / contestName;
            SystemHelper::openInCode(fullPath.string());
        }

        if (initGitRepoOption.get_active()) {
            const std::filesystem::path fullPath = std::filesystem::path(folderName) / contestName;
            SystemHelper::createGitRepo(fullPath.string());
        }

        resetForm();
    }
    catch (const std::exception& e) {
        showErrorDialog("Помилка", e.what());
    }
}

void MainWindow::validateFormInputs(const std::string& contestName) const {
    if (contestName.empty()) {
        throw std::invalid_argument("Ви не вказали назву контесту");
    }

    if (folderName.empty()) {
        throw std::invalid_argument("Ви не вказали шлях до директорії");
    }

    static const std::string FORBIDDEN_CHARS = "<>:\"/\\|?*";
    for (char c : contestName) {
        if (FORBIDDEN_CHARS.find(c) != std::string::npos) {
            throw std::invalid_argument("Назва контесту містить заборонений символ: " + c);
        }
    }
}

void MainWindow::resetForm() {
    contestNameEntry.set_text("");
    folderName.clear();
    problemCountSelector.set_active(DEFAULT_COMBO_SELECTION);
    directoryPicker.set_label("Обрати папку");
    openInVSCodeOption.set_active(false);
    initGitRepoOption.set_active(false);
}

void MainWindow::showErrorDialog(const std::string& title, const std::string& subTitle) {
    Gtk::MessageDialog dialog(*this, title, false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
    dialog.set_secondary_text(subTitle);
    dialog.run();
}

void MainWindow::showSuccessDialog(const std::string& title, const std::string& subTitle) {
    Gtk::MessageDialog dialog(*this, title, false, Gtk::MESSAGE_INFO);
    dialog.set_secondary_text(subTitle);
    dialog.run();
}
