#include "core/TemplateManager.hpp"
#include <core/ConfigManager.hpp>

#include <cctype>

std::string TemplateManager::getLanguageTemplate(const std::string& langName, const std::string& className) {
	auto languages = ConfigManager::loadConfig();
	for (const auto& lang : languages) {
		if (lang.name == langName && lang.enabled) {
			std::string template_code = lang.getActiveTemplate();
			const std::string placeholder = "{CLASS_NAME}";
			size_t pos = 0;
			while ((pos = template_code.find(placeholder, pos)) != std::string::npos) {
				template_code.replace(pos, placeholder.length(), className);
				pos += className.length();
			}
			return template_code;
		}
	}
	return "";
}

std::vector<LanguageTemplate> TemplateManager::getActiveLanguages() {
	auto languages = ConfigManager::loadConfig();
	std::vector<LanguageTemplate> enabled;
	for (const auto& lang : languages) {
		if (lang.enabled) {
			enabled.push_back(lang);
		}
	}
	return enabled;
}

Glib::ustring TemplateManager::getReadmeTemplate(const std::string& projectName) {
    return Glib::ustring::compose("# %1\n", projectName);
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