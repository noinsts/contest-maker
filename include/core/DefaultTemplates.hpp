#pragma once

#include <string>
#include <vector>
#include <core/LanguageTemplate.hpp>

class DefaultTemplates {
public:
	[[nodiscard]] static std::vector<LanguageTemplate> getAllTemplates();

private:
	[[nodiscard]] static std::string getPythonDefaultTemplate();
	[[nodiscard]] static std::string getJavaDefaultTemplate();
	[[nodiscard]] static std::string getCppDefaultTemplate();
};