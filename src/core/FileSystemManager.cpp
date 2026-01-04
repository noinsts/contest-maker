#include "core/FileSystemManager.hpp"

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace fs = std::filesystem;

void FileSystemManager::createDirectory(const fs::path& path) {
	try {
		if (fs::create_directories(path)) {
			std::cout << "Created directory: " << path.string() << std::endl;
		}
		else {
			std::cout << "Directory already exists or was already there: " << path.string() << std::endl;
		}
	}
	catch (const fs::filesystem_error& e) {
		throw std::runtime_error("Failed to create directory: " + path.string()  + " - " + e.what());
	}
}

void FileSystemManager::createFile(const fs::path& path, const std::string& context) {
	try {
		if (fs::exists(path)) {
			std::cout << "File already exists, skipping: " << path.string() << std::endl;
			return;
		}

		if (path.has_parent_path() && !fs::exists(path.parent_path())) {
			createDirectory(path.parent_path());
		}

		std::ofstream file(path);
		if (!file.is_open()) {
			throw std::runtime_error("Cannot open file for writing" + path.string());
		}

		file << context;

		if (!file.good()) {
			throw std::runtime_error("Error writing to file: " + path.string());
		}

		file.close();
		std::cout << "Created file: " << path.string() << std::endl;
	}
	catch (const std::exception& e) {
		throw std::runtime_error("Failed to create file: " + path.string() + " - " + e.what());
	}
}
