#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>
#include <stdexcept>

class KatagoLink {
public:
    KatagoLink() = default;
    ~KatagoLink() { stop(); }

    KatagoLink(const KatagoLink&) = delete;
    KatagoLink& operator=(const KatagoLink&) = delete;

    void start(const std::string& exeDir,
        const std::string& exeName,
        const std::string& modelPath,
        const std::string& cfgPath,
        bool hideWindow = true);

    void stop();

    // Send one GTP command (newline is added automatically)
    void send(const std::string& gtpLine);

    // Read one full GTP response (until "\n\n")
    std::string readResponse();

    bool isRunning() const { return hProcess != nullptr; }

private:
    HANDLE hProcess = nullptr;
    HANDLE hThread = nullptr;
    HANDLE hStdinWrite = nullptr;
    HANDLE hStdoutRead = nullptr;

    std::string readBuf;

    static void throwLastError(const char* what);
};
