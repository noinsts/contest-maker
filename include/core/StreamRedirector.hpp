#pragma once

#include <streambuf>
#include <ostream>
#include <string>
#include <mutex>

class StreamRedirector : public std::streambuf {
public:
    explicit StreamRedirector(std::ostream& stream);
    ~StreamRedirector();

    static std::string getCapturedText();
    static void cleanCapturedText();
    static bool isCapturing();

protected:
    int overflow(int c) override;

private:
    std::ostream& targetStream;
    std::streambuf* originalStreamBuffer;

    static std::string capturedText;
    static std::mutex textMutex;
    static bool capturing;
};
