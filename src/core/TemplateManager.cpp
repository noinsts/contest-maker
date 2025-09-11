#include "core/TemplateManager.hpp"

#include <cctype>

std::string TemplateManager::getCppTemplate() {
    return R"(

    #include <bits/stdc++.h>
    using namespace std;

    void solve() {
        // TODO: code
    }

    int main() {
        ios_base::sync_with_stdio(false):
        cin.tie(nullptr);

        int tt; cin >> tt;

        while (tt--) {
            solve();
        }

        return 0;
    }
    )";
}

std::string TemplateManager::getJavaTemplate() {
    return "";
}

std::string TemplateManager::getPythonTemplate() {
    return (
        "def main() -> None:\n"
        "   pass\n\n"
        "if __name__ == \"__main__\":\n"
        "   main()"
    );
}

std::string TemplateManager::getReadmeTemplate(const std::string& projectName) {
    return "# " + projectName;
}

std::string TemplateManager::getCMakeTemplate(const std::string& projectName, const char& maxSubDirs) {
    std::string cmake;

    cmake += "cmake_minumum_required(VERSION 3.26)\n\n";
    cmake += "project(" + projectName + ")\n\n";
    cmake += "set(CMAKE_CXX_STANDARD 17)\n\n";

    for (char c = 'A'; c <= maxSubDirs; ++c) {
        std::string exeName(1, std::tolower(c));
        std::string srcPath = "src/" + std::string(1, c) + "/" + std::string(1, c) + ".cpp";
        cmake += "add_executable(" + exeName + " " + srcPath + ")\n";
    }

    return cmake;
}