#pragma once

#include <string>

/**
 * @struct LanguageTemplate
 * @brief Структура для зберігання інформації про мову програмування та її
 * шаблону.
 */
struct LanguageTemplate {
	std::string name;		///< Назва мови програмування.
	std::string extension; ///< Формат мови.
	std::string defaultTemplate; ///< Дефолтний шаблон.
	std::string customTemplate; ///< Кастомний шаблон.
	bool enabled; ///< Чи ввімкнена ця мова.

	/**
	 * @brief Повертає шаблон мови програмування.
	 */
	[[nodiscard]] std::string getActiveTemplate() const {
		return customTemplate.empty() ? defaultTemplate : customTemplate;
	}

	/**
	 * @brief Скидає шаблон мови до дефолтного.
	 */
	void resetToDefault() {
		customTemplate.clear();
	}

	/**
	 * @brief Чи має мова кастомний шаблон.
	 */
	[[nodiscard]] bool hasCustomTemplate() const {
		return !customTemplate.empty();
	}
};