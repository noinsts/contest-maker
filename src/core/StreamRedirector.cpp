#include "core/StreamRedirector.hpp"
#include <iostream>

std::string StreamRedirector::capturedText = "";
std::mutex StreamRedirector::textMutex;
bool StreamRedirector::capturing = false;

StreamRedirector::StreamRedirector(std::ostream& stream)
    : targetStream(stream), originalStreamBuffer(stream.rdbuf()) {
    std::lock_guard<std::mutex> lock(textMutex);
    capturing = true;
    stream.rdbuf(this);
}

StreamRedirector::~StreamRedirector() {
    std::lock_guard<std::mutex> lock(textMutex);
    targetStream.rdbuf(originalStreamBuffer);
    capturing = false;
}

int StreamRedirector::overflow(int c) {
    if (c != EOF) {
        char ch = static_cast<char>(c);

        {
            std::lock_guard<std::mutex> lock(textMutex);
            capturedText += ch;
        }

        originalStreamBuffer->sputc(c);
    }
    return c;
}

std::string StreamRedirector::getCapturedText() {
    std::lock_guard<std::mutex> lock(textMutex);
    return capturedText;
}

void StreamRedirector::cleanCapturedText() {
    std::lock_guard<std::mutex> lock(textMutex);
    capturedText.clear();
}

bool StreamRedirector::isCapturing() {
    std::lock_guard<std::mutex> lock(textMutex);
    return capturing;
}
