#pragma once

#include <streambuf>
#include <ostream>
#include <string>
#include <mutex>

/**
 * @class StreamRedirector
 * @brief Перенаправляє вивід з std::ostream (наприклад std::cout).
 * 
 * Використовується для відладки.
 */
class StreamRedirector : public std::streambuf {
public:
    /**
     * @brief Конструктор StreamRedirector для конкретного потоку виводу.
     * @param stream Потік виводу, який потрібно перенаправити (наприклад, std::cout).
     */
    explicit StreamRedirector(std::ostream& stream);

    /**
     * @brief Деструктор відновлює оригінальний буфер потоку.
     */
    ~StreamRedirector();

    /**
     * @brief Повертає весь захоплений текст.
     */
    static std::string getCapturedText();
    
    /**
     * @brief Очищає буфер захопленого тексту
     */
    static void cleanCapturedText();

    /**
     * @brief Перевіряє, чи є наразі захоплення потоку.
     * @return true, якщо захоплення активне, false - якщо ні.
     */
    static bool isCapturing();

protected:
    /**
     * @brief Перевизначений метод std::streambuf для обробки кожного символу виводу.
     * @param c Символ для виводу.
     * @return Символ, що було записано, або EOF у разі помилки.
     */
    int overflow(int c) override;

private:
    std::ostream& targetStream;             ///< Потік, що захоплюється.
    std::streambuf* originalStreamBuffer;   ///< Оригінальний буфер потоку.

    static std::string capturedText;        ///< Буфер для захоплення тексту.
    static std::mutex textMutex;            ///< М'ютекс для потокобезпечності.
    static bool capturing;                  ///< Прапорець активного захоплення.
};
