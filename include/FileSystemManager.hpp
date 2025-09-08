#pragma once

#include <string>
#include <filesystem>

/**
 * @class FileSystemManager.
 * @brief Клас для роботи з файловою системою.
 */
class FileSystemManager {
public:
    /**
     * @brief Метод створює директорію.
     * @param path Шлях до директорії.
     * @throws runtime_error Якщо виникла помилка.
     */
    static void createDirectory(const std::filesystem::path& path);

    /**
     * @brief Створює файл.
     * @param path Глях до файлу.
     * @param context Вміст файлу.
     * @throws runtime_error Якщо виникла помилка.
     */
    static void createFile(const std::filesystem::path& path, const std::string& context);
};
