#include "core/ConfigManager.hpp"

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <nlohmann/json.hpp>
#include <core/DefaultTemplates.hpp>

using json = nlohmann::json;

std::vector<LanguageTemplate> ConfigManager::loadConfig() {
	auto configPath = getTemplatePath();
	if (!std::filesystem::exists(configPath)) {
		return DefaultTemplates::getAllTemplates();
	}
	try {
		std::ifstream file(configPath);
		if (!file.is_open()) {
			throw std::runtime_error("Cannot open config file");
		}
		json j;
		file >> j;
		return fromJson(j.dump());
	}
	catch (const std::exception& e) {
		std::cerr << "Error load config: " << e.what() << std::endl
				  << "Using default templates" << std::endl;
		return DefaultTemplates::getAllTemplates();
	}
}

void ConfigManager::saveConfig(const std::vector<LanguageTemplate>& languages) {
	auto path = getTemplatePath();
	std::error_code ec;
	std::filesystem::create_directories(path.parent_path(), ec);
	std::ofstream file(path, std::ios::out | std::ios::trunc);
	if (!file) {
		std::cerr << "Failed to save config to " << path.string() << std::endl;
		return;
	}
	file << toJson(languages);
}

std::vector<LanguageTemplate> ConfigManager::fromJson(const std::string& raw) {
	json j = json::parse(raw);
	std::vector<LanguageTemplate> languages;
	if (!j.contains("languages") || !j["languages"].is_array()) {
		return languages;
	}
	auto defaults = DefaultTemplates::getAllTemplates();
	for (const auto& item : j["languages"]) {
		LanguageTemplate lang;
		lang.name = item.value("name", "");
		lang.extension = item.value("extension", "");
		lang.customTemplate = item.value("customTemplate", "");
		lang.enabled = item.value("enabled", true);
		for (const auto& def : defaults) {
			if (def.name == lang.name) {
				lang.defaultTemplate = def.defaultTemplate;
				break;
			}
		}
		languages.push_back(lang);
	}
	return languages;
}

std::string ConfigManager::toJson(const std::vector<LanguageTemplate>& languages) {
	json j = json::object();
	j["languages"] = json::array();

	for (const auto& lang : languages) {
		j["languages"].push_back({
			{"name", lang.name},
			{"extension", lang.extension},
			{"enabled", lang.enabled},
			{"customTemplate", lang.customTemplate}
		});
	}
	return j.dump(4);
}

void ConfigManager::ensureTemplateFileExists() {
	std::filesystem::path templatePath = getTemplatePath();
	std::error_code ec;
	std::filesystem::create_directories(templatePath.parent_path(), ec);
	if (ec) {
		std::cerr << "Failed to create config directory: " << ec.message() << std::endl;
		return;
	}
	if (!std::filesystem::exists(templatePath)) {
		std::ofstream file(templatePath);
		if (!file) {
			std::cerr << "Failed to create template.json" << std::endl;
			return;
		}
		file <<
R"({
  "languages": []
}
)";
	}
}

std::filesystem::path ConfigManager::getTemplatePath() {
	std::filesystem::path directoryPath;
#ifdef _WIN32
	if (const char* appdata = std::getenv("APPDATA")) {
		directoryPath = std::filesystem::path(appdata) / "contest-maker";
	} else {
		directoryPath = std::filesystem::path("C:/contest-maker/");
	}
#else
	if (const char* home = std::getenv("HOME")) {
		directoryPath = std::filesystem::path(home) / ".config" / "contest-maker";
	} else {
		directoryPath = "./contest-maker";
	}
#endif
	return directoryPath / "template.json";
}