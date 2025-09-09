#pragma once
#include <string>

/**
 * @class SystemHelper
 * @brief Клас з допоміжних методів керування системою
 */
class SystemHelper {
public:
	/**
	 * @brief Метод відкриває директорію в Visual Studio Code
	 * @param fullPath шлях до директорії
	 * @throws runtime_error Якщо не вдається відкрити Code.
	 */
	static void openInCode(const std::string& fullPath);
	
	/**
	 * @brief Метод створює Git-репозиторій
	 * @param fullPath шлях до директорії
	 * @throws runtime_error Якщо не вдається створити Git-репозиторій.
	*/
	static void createGitRepo(const std::string& fullPath);
};
