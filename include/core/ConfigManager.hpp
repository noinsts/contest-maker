#pragma once

#include <core/LanguageTemplate.hpp>
#include <filesystem>
#include <string>
#include <vector>

class ConfigManager {
public:
	static std::vector<LanguageTemplate> loadConfig();
	static void saveConfig(const std::vector<LanguageTemplate>& languages);
	static void ensureTemplateFileExists();
private:
	static std::filesystem::path getTemplatePath();
	static std::vector<LanguageTemplate> fromJson(const std::string& raw);
	static std::string toJson(const std::vector<LanguageTemplate>& languages);
};