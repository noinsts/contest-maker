#include <core/SystemHelper.hpp>

#include <string>
#include <stdexcept>

void SystemHelper::openInCode(const std::string& fullPath) {
    std::string command;

    #ifdef _WIN32
        command = "start \"\" \"%LOCALAPPDATA%\\Programs\\Microsoft VS Code\\Code.exe\" \"" + fullPath + "\"";
    #elif __APPLE__
        command = "open -a \"Visual Studio Code\" \"" + fullPath + "\"";
    #else
        command = "code \"" + fullPath + "\" &";
    #endif

    if (std::system(command.c_str()) != 0) {
        throw std::runtime_error("Не вдалося відкрити Code");
    }
}

void SystemHelper::createGitRepo(const std::string& fullPath) {
    std::string escapedPath = "\"" + fullPath + "\"";
    const std::string command = "cd " + escapedPath + " && git init";

    if (std::system(command.c_str()) != 0) {
        throw std::runtime_error("Помилка під час створення Git репозиторію");
    }
}
