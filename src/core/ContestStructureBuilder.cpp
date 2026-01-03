#include "core/ContestStructureBuilder.hpp"

#include <string>
#include <filesystem>

#include <core/FileSystemManager.hpp>
#include <core/TemplateManager.hpp>

ContestStructureBuilder::ContestStructureBuilder(
    const std::string& contestName,
    char maxProblemLetter,
    const std::filesystem::path& targetDirectory
)
    : contestName_(contestName),
    maxProblemLetter_(maxProblemLetter),
    targetDirectory_(targetDirectory) {}

std::filesystem::path ContestStructureBuilder::getContestPath() const {
    return targetDirectory_ / contestName_;
}

std::filesystem::path ContestStructureBuilder::getProblemPath(char problemLetter) const {
    return getContestPath() / "src" / std::string(1, problemLetter);
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
    FileSystemManager::createDirectory(getContestPath() / "src");
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

    const auto enabledLanguages = TemplateManager::getActiveLanguages();
    for (const auto& lang : enabledLanguages) {
        const std::string& templateCode = TemplateManager::getLanguageTemplate(lang.name, fileName);
        if (!templateCode.empty()) {
            FileSystemManager::createFile(
                problemDir / (fileName + lang.extension),
                templateCode
            );
        }
    }
}

void ContestStructureBuilder::build() const {
    createContestDirectory();
    generateProjectFiles();
    createProblemDirectories();
    generateSourceFiles();
}
