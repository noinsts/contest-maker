#pragma once

#include <glibmm/ustring.h>
#include <string>
#include <vector>
#include <core/LanguageTemplate.hpp>

class TemplateManager {

public:
	static std::string getLanguageTemplate(const std::string& langName, const std::string& className = "");

	static std::vector<LanguageTemplate> getActiveLanguages();

    /**
    * @brief Шаблон для README.
    * @param projectName Назва контесту.
    */
    static Glib::ustring getReadmeTemplate(const std::string& projectName);

    /**
    * @brief Шаблон для CMakeLists.
    * @param projectName Назва контесту.
    * @param maxSubDirs Кількість задач.
    */
    static std::string getCMakeTemplate(const std::string& projectName, const char& maxSubDirs);
};
