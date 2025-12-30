#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <core/LanguageTemplate.hpp>

/**
 * @class ConfigManager.
 * @brief Керує конфігураційним файлом шаблонів мов програмування.
 * 
 * Клас відповідає за зчитування та збереження файлу `template.json`,
 * який містить список LanguageTemplate.
 * 
 * Поведінка:
 * - Якщо конфігураційний файл відсутній або пошкоджений - 
 *		використовуються стандатні шаблони з DefaultTemplates.
 * - Помилки вводу/виводу та JSON-парсингу не призводять до аварійного завершення.
 * 
 * Формат файлу:
 * @code{.json}
 * {
 *   "languages": [
 *     {
 *       "name": "C++",
 *       "extension": ".cpp",
 *       "enabled": true,
 *       "customTemplate": "..."
 *     }
 *   ]
 * }
 * @endcode
 */
class ConfigManager {
public:
	/**
	 * @brief Завантажує конфігурацію шаблонів із файлу.
	 * 
	 * Якщо файлу не існує, не може бути відкритий або містить некоректний JSON,
	 * повертається список стандартних шаблонів.
	 * 
	 * @return Список мов програмування.
	 */
	static std::vector<LanguageTemplate> loadConfig();

	/**
	 * @brief Зберігає конфігурацію шаблонів у файл.
	 * 
	 * Автоматично створює всі необхідні директорії.
	 * У разі помилки запису дані не зберігаються.
	 * 
	 * @param languages Список мов рограмування для зберігання.
	 */
	static void saveConfig(const std::vector<LanguageTemplate>& languages);

	/**
	 * @brief Перевіряє існування конфігураційного файлу та створює його за потреби.
	 * 
	 * Якщо файлу не існує, створюється новий файл `template.json` з базовою структурою JSON.
	 */
	static void ensureTemplateFileExists();

private:
	/**
	 * @brief Повертає шлях до файлу конфігурації шаблонів.
	 * 
	 * Платформизалежна поведінка:
	 * - Windows: %APPDATA%/contest-maker/template.json
	 * - Linux/macOS: ~/.config/contest-maker/template.json
	 * 
	 * @return Шлях до файлу конфігурації.
	 */
	static std::filesystem::path getTemplatePath();

	/**
	 * @brief Парсить JSON-рядок у список LanguageTemplate.
	 * 
	 * @param raw Сирий JSON-рядок.
	 * @return Список мов програмування.
	 */
	static std::vector<LanguageTemplate> fromJson(const std::string& raw);

	/**
	 * @brief Серіалізує список LanguageTemplate у JSON-рядок.
	 * 
	 * @param languages Список мов програмування.
	 * @return Відформатований JSON-рядок.
	 */
	static std::string toJson(const std::vector<LanguageTemplate>& languages);
};