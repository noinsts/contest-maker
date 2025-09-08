#include "FileSystemManager.hpp"

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <stdexcept>

void FileSystemManager::createDirectory(const std::filesystem::path& path) {
	try {
		if (std::filesystem::exists(path)) {
			std::cout << "Directory already exists, skipping: " << path.string() << std::endl;
			return;
		}

		if (std::filesystem::create_directories(path)) {
			std::cout << "Created directory: " << path.string() << std::endl;
		}
	}
	catch (const std::filesystem::filesystem_error& e) {
		throw std::runtime_error("Failed to create directory: " + path.string()  + " - " + e.what());
	}
}

void FileSystemManager::createFile(const std::filesystem::path& path, const std::string& context) {
	try {
		if (std::filesystem::exists(path)) {
			std::cout << "File already exists, skipping: " << path.string() << std::endl;
			return;
		}

		std::ofstream file(path);
		if (!file.is_open()) {
			throw std::runtime_error("Cannot open file for writing" + path.string());
		}

		file << context;
		if (file.fail()) {
			throw std::runtime_error("Error writing to file: " + path.string());
		}

		std::cout << "Created file: " << path.string() << std::endl;
	}
	catch (const std::exception& e) {
		throw std::runtime_error("Failed to create file: " + path.string() + " - " + e.what());
	}
}
