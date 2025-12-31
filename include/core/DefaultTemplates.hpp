#pragma once

#include <string>
#include <vector>
#include <core/LanguageTemplate.hpp>

/**
 * @class DefaultTemplates.
 * @brief Клас, що надає стандартні шаблони для мов програмування.
 * 
 * Містить набір статичних методів для отримання
 * дефолтних шаблонів коду (Python, Java, C++).
 * Використовується як джерело базових LanguageTemplate.
 */
class DefaultTemplates {
public:
	/**
	 * @brief Повертає всі доступні стандартні шаблони мов програмування.
	 * 
	 * Формує та повертає список об'єктів LanguageTemplate
	 * для кожної підтримуваної мови.
	 * 
	 * @return Вектор стандартних шаблонів.
	 */
	[[nodiscard]] static std::vector<LanguageTemplate> getAllTemplates();

private:
	[[nodiscard]] static std::string getPythonDefaultTemplate();
	[[nodiscard]] static std::string getJavaDefaultTemplate();
	[[nodiscard]] static std::string getCppDefaultTemplate();
};