#include "core/ContestStructureBuilder.hpp"

#include <string>
#include <filesystem>
#include <utility>

#include <core/FileSystemManager.hpp>
#include <core/TemplateManager.hpp>

ContestStructureBuilder::ContestStructureBuilder(std::string contestName, char maxProblemLetter, std::filesystem::path targetDirectory)
    : contestName_(std::move(contestName)),
    maxProblemLetter_(maxProblemLetter),
    targetDirectory_(std::move(targetDirectory)) {
}

std::filesystem::path ContestStructureBuilder::getContestPath() const {
    return targetDirectory_ / contestName_;
}

std::filesystem::path ContestStructureBuilder::getProblemPath(char problemLetter) const {
    return getContestPath() / std::string{ problemLetter };
}

void ContestStructureBuilder::createContestDirectory() const {
    FileSystemManager::createDirectory(getContestPath());
}

void ContestStructureBuilder::generateProjectFiles() const {
    FileSystemManager::createFile(
        getContestPath() / "CMakeLists.txt",
        TemplateManager::getCMakeTemplate(contestName_, maxProblemLetter_)
    );
    FileSystemManager::createFile(
        getContestPath() / "README.md",
        TemplateManager::getReadmeTemplate(contestName_)
    );
}

void ContestStructureBuilder::createProblemDirectories() const {
    for (char problemLetter = 'A'; problemLetter <= maxProblemLetter_; ++problemLetter) {
        FileSystemManager::createDirectory(getProblemPath(problemLetter));
    }
}

void ContestStructureBuilder::generateSourceFiles() const {
    for (char problemLetter = 'A'; problemLetter <= maxProblemLetter_; ++problemLetter) {
        generateSourceFilesForProblem(problemLetter);
    }
}

void ContestStructureBuilder::generateSourceFilesForProblem(char problemLetter) const {
    const std::filesystem::path problemDir = getProblemPath(problemLetter);
    const std::string fileName{ problemLetter };
    FileSystemManager::createFile(
        problemDir / (fileName + ".cpp"),
        TemplateManager::getCppTemplate()
    );
    FileSystemManager::createFile(
        problemDir / (fileName + ".py"),
        TemplateManager::getPythonTemplate()
    );
}

void ContestStructureBuilder::build() const {
    createContestDirectory();
    generateProjectFiles();
    createProblemDirectories();
    generateSourceFiles();
}
