#pragma once

#include <string>
#include <vector>
#include <glibmm/ustring.h>
#include <core/LanguageTemplate.hpp>

/**
 * @class TemplateManager.
 * @brief Клас для роботи з шаблонами мов програмування, README та CMake.
 * 
 * Цей клас надає статичні методи для отримання шаблонів коду,
 * списку активних мов програмування, шаблону README та CMakeLists.
 */
class TemplateManager {
public:
    /**
     * @brief Повертає шаблон коду для заданої мови програмування.
     * 
     * Якщо передано className, то всі {CLASS_NAME} у шаблоні
     * замінюються на значення className.
     * 
     * @param langName Назва мови програмування.
     * @param className Назва класу, яка замінить {CLASS_NAME} у шаблоні (необов'язково)
     * @return Шаблон коду у вигляді рядка.
     */
    static std::string getLanguageTemplate(const std::string& langName, const std::string& className = "");

    /**
     * @brief Повертає список ввімкнених (активних) мов програмування.
     * @return Вектор об'єктів LanguageTemplate.
     */
    static std::vector<LanguageTemplate> getActiveLanguages();

    /**
    * @brief Повертає шаблон README для проєкту.
    * @param projectName Назва проєкту або контесту.
    * @return Шаблон README у вигляді рядка.
    */
    static Glib::ustring getReadmeTemplate(const std::string& projectName);

    /**
    * @brief Повертає шаблон CMakeLists.txt для проєкту.
    * @param projectName Назва проєкту.
    * @param maxSubDirs Кількість піддиректорій із задачами.
    */
    static std::string getCMakeTemplate(const std::string& projectName, const char& maxSubDirs);
};
