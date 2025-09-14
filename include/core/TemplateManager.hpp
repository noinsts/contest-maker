#pragma once
#include <glibmm/ustring.h>
#include <string>

class TemplateManager {

public: 
    /**
    * @brief Шаблон для С++.
    */
    static Glib::ustring getCppTemplate();

    /**
    * @brief Шаблон для Java.
    */
    static Glib::ustring getJavaTemplate(const std::string& className);

    /**
    * @brief Шаблон для Python.
    */
    static Glib::ustring getPythonTemplate();

    /**
    * @brief Шаблон для README.
    */
    static std::string getReadmeTemplate(const std::string& projectName);

    /**
    * @brief Шаблон для CMakeLists.
    */
    static std::string getCMakeTemplate(const std::string& projectName, const char& maxSubDirs);
};
