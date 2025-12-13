#include "AI/KatagoLink.h"
#include <vector>

static std::wstring toW(const std::string& s) {
    if (s.empty()) return L"";
    int n = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), nullptr, 0);
    std::wstring w(n, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), w.data(), n);
    return w;
}

void KatagoLink::throwLastError(const char* what) {
    DWORD err = GetLastError();
    char buf[256];
    snprintf(buf, sizeof(buf), "%s (GetLastError=%lu)", what, (unsigned long)err);
    throw std::runtime_error(buf);
}

void KatagoLink::start(const std::string& exeDir,
    const std::string& exeName,
    const std::string& modelPath,
    const std::string& cfgPath,
    bool hideWindow)
{
    stop();

    SECURITY_ATTRIBUTES sa{};
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;

    HANDLE stdinRead = nullptr, stdinWrite = nullptr;
    HANDLE stdoutRead = nullptr, stdoutWrite = nullptr;

    if (!CreatePipe(&stdinRead, &stdinWrite, &sa, 0))
        throwLastError("CreatePipe(stdin)");
    if (!SetHandleInformation(stdinWrite, HANDLE_FLAG_INHERIT, 0))
        throwLastError("SetHandleInformation(stdinWrite)");

    if (!CreatePipe(&stdoutRead, &stdoutWrite, &sa, 0))
        throwLastError("CreatePipe(stdout)");
    if (!SetHandleInformation(stdoutRead, HANDLE_FLAG_INHERIT, 0))
        throwLastError("SetHandleInformation(stdoutRead)");

    STARTUPINFOW si{};
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = stdinRead;
    si.hStdOutput = stdoutWrite;
    si.hStdError = stdoutWrite;

    PROCESS_INFORMATION pi{};

    std::string cmd =
        "\"" + exeName + "\" gtp -model \"" + modelPath + "\" -config \"" + cfgPath + "\"";

    std::wstring wCmd = toW(cmd);
    std::wstring wDir = toW(exeDir);

    DWORD flags = hideWindow ? CREATE_NO_WINDOW : 0;

    BOOL ok = CreateProcessW(
        nullptr,
        wCmd.data(),
        nullptr, nullptr,
        TRUE,
        flags,
        nullptr,
        wDir.empty() ? nullptr : wDir.c_str(),
        &si,
        &pi
    );

    CloseHandle(stdinRead);
    CloseHandle(stdoutWrite);

    if (!ok) {
        CloseHandle(stdinWrite);
        CloseHandle(stdoutRead);
        throwLastError("CreateProcessW(katago)");
    }

    hProcess = pi.hProcess;
    hThread = pi.hThread;
    hStdinWrite = stdinWrite;
    hStdoutRead = stdoutRead;
    readBuf.clear();
}

void KatagoLink::stop() {
    if (!hProcess) return;

    try { send("quit"); }
    catch (...) {}

    if (hStdinWrite) { CloseHandle(hStdinWrite); hStdinWrite = nullptr; }
    if (hStdoutRead) { CloseHandle(hStdoutRead); hStdoutRead = nullptr; }

    WaitForSingleObject(hProcess, 200);

    if (hThread) { CloseHandle(hThread); hThread = nullptr; }
    CloseHandle(hProcess);
    hProcess = nullptr;
}

void KatagoLink::send(const std::string& gtpLine) {
    if (!hStdinWrite)
        throw std::runtime_error("KatagoLink not started");

    std::string s = gtpLine + "\n";
    DWORD written = 0;
    if (!WriteFile(hStdinWrite, s.data(), (DWORD)s.size(), &written, nullptr))
        throwLastError("WriteFile(stdin)");
}

std::string KatagoLink::readResponse() {
    if (!hStdoutRead)
        throw std::runtime_error("KatagoLink not started");

    auto hasGtpHeaderLine = [&](const std::string& s) -> bool {
        for (size_t i = 0; i < s.size(); i++) {
            if ((i == 0 || s[i - 1] == '\n') && (s[i] == '=' || s[i] == '?'))
                return true;
        }
        return false;
        };

    auto findEndMarker = [&](const std::string& s) -> size_t {
        size_t a = s.find("\n\n");
        size_t b = s.find("\r\n\r\n");
        if (a == std::string::npos) return b;
        if (b == std::string::npos) return a;
        return (a < b) ? a : b;
        };

    bool sawHeader = false;

    while (true) {
        if (!sawHeader && hasGtpHeaderLine(readBuf))
            sawHeader = true;

        if (sawHeader) {
            size_t end = findEndMarker(readBuf);
            if (end != std::string::npos) {
                size_t len = (readBuf.compare(end, 4, "\r\n\r\n") == 0) ? 4 : 2;
                std::string resp = readBuf.substr(0, end + len);
                readBuf.erase(0, end + len);
                return resp;
            }
        }

        char tmp[1024];
        DWORD got = 0;
        if (!ReadFile(hStdoutRead, tmp, sizeof(tmp), &got, nullptr))
            throwLastError("ReadFile(stdout)");
        if (got == 0) break;
        readBuf.append(tmp, tmp + got);
    }

    std::string all = readBuf;
    readBuf.clear();
    return all;
}
