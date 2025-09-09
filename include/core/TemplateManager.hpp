#pragma once
#include <string>

class TemplateManager {

public: 
    /**
    * @brief Шаблон для С++.
    */
    static std::string getCppTemplate();

    /**
    * @brief Шаблон для Java.
    */
    static std::string getJavaTemplate();

    /**
    * @brief Шаблон для Python.
    */
    static std::string getPythonTemplate();

    /**
    * @brief Шаблон для README.
    */
    static std::string getReadmeTemplate(const std::string& projectName);

    /**
    * @brief Шаблон для CMakeLists.
    */
    static std::string getCMakeTemplate(const std::string& projectName, const char& maxSubDirs);
};
