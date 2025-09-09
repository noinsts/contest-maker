#pragma once

#include <filesystem>
#include <string>

class ContestStructureBuilder {

private:
	std::string contestName_;
	char maxProblemLetter_;
	std::filesystem::path targetDirectory_;

	/**
	 * @brief Повертає повний шлях до контесту.
	 * @returns Повний шлях до контесту.
	 */
	[[nodiscard]] std::filesystem::path getContestPath() const;

	/**
	* @brief Повертає шлях до конкретної задачі
	* @param problemLetter Літера задачі.
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
	* @brief Створює сурс файли для конкретної задачі.
	* @param problemLetter Літера задачі
	*/
	void generateSourceFilesForProblem(char problemLetter) const;

public:
	/**
	 * @brief Конструктор класу ContestStructureBuilder.
	 * @param contestName Назва контесту
	 * @param maxProblemLetter Остання літера задачі (A, B, C, ...)
	 * @param targetDirectory Цільова директорія для створення
	 */
	ContestStructureBuilder(std::string contestName, char maxProblemLetter, std::filesystem::path targetDirectory);

	/**
	* @brief Створює повну архітектуру контесту.
	* @throws runtime_error Якщо виникла помилка при створенні.
	*/
	void build() const;
};
