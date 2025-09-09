#pragma once
#include <string>

class TemplateManager {
    public: 
        static std::string getCppTemplate();
        static std::string getJavaTemplate();
        static std::string getPythonTemplate();
        static std::string getReadmeTemplate(const std::string& projectName);
        static std::string getCMakeTemplate(const std::string& projectName, const char& maxSubDirs);
};
