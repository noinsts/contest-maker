#pragma once

#include <filesystem>
#include <string>

/**
 * @class ContestStructureBuilder
 * @brief Відповідає за створення файлової структури контесту 
 * з задачами FIRST_PROBLEM..maxProblemLetter_.
 * 
 * Клас генерує директорії, базові конфігураційні файли
 * та шаблонні source-файли для кожної задачі.
 */
class ContestStructureBuilder {
private:
	const std::string& contestName_; ///< Назва контесту.
	char maxProblemLetter_; ///< Остання літера задачі.
	const std::filesystem::path& targetDirectory_; ///< Коренева директорія створення.
	static constexpr char FIRST_PROBLEM = 'A'; ///< Перша допустима літера задачі.
	static constexpr char LAST_PROBLEM = 'Z'; ///< Остання допустима літера задачі.

	/**
	 * @brief Повертає повний шлях до контесту.
	 * @return Повний шлях до контесту.
	 */
	[[nodiscard]] std::filesystem::path getContestPath() const;

	/**
	* @brief Повертає шлях до конкретної задачі
	* @param problemLetter Літера задачі в діапазоні FIRST_PROBLEM-maxProblemLetter_.
	* @return Повний шлях до директорії задачі.
	*/
	[[nodiscard]] std::filesystem::path getProblemPath(char problemLetter) const;

	/**
	* @brief Створює головну директорію контесту.
	*/
	void createContestDirectory() const;

	/**
	* @brief Генерує основні файли проекту (README.md, CMakeLists.txt, тощо).
	*/
	void generateProjectFiles() const;

	/**
	* @brief Створює директорії для всіх задач (A, B, C, ...)
	*/
	void createProblemDirectories() const;

	/**
	* @brief Генерує вихідні дані для всіх задач.
	*/
	void generateSourceFiles() const;


	/**
	* @brief Створює source-файли для конкретної задачі.
	* @param problemLetter Літера задачі
	*/
	void generateSourceFilesForProblem(char problemLetter) const;

	/**
	 * @brief Перевіряє, чи входить maxProblemLetter у діапазоні FIRST_PROBLEM-LAST_PROBLEM.
	 * @throws std::invalid_argument При невдалій валідації.
	 */
	void validateMaxProblemLetter() const;

public:
	/**
	 * @brief Конструктор класу ContestStructureBuilder.
	 * @param contestName Назва контесту
	 * @param maxProblemLetter Остання літера задачі. Має бути в діапазоні 'A'-'Z'.
	 * @param targetDirectory Цільова директорія для створення
	 */
	ContestStructureBuilder(
		const std::string& contestName, 
		char maxProblemLetter, 
		const std::filesystem::path& targetDirectory
);

	/**
	* @brief Створює повну архітектуру контесту.
	* @throws std::exception Якщо будь-який з етапів створення завершився помилкою.
	*/
	void build() const;
};
