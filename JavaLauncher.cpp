#include "JavaLauncher.h"
#include "MessageBox.h"

#include <iostream>
#include <regex>
#include <string>

#if WIN32
#include <windows.h>
#endif

void LaunchJar(const std::string& jar_path) {
    if (IsJavaVersionCorrect()) {
        std::string command = "java -jar " + jar_path;
        std::system(command.c_str());
    } else {
        std::string message = "Java version " + std::to_string(MINIMUM_JAVA_MAJOR_VERSION_REQUIRED) + " or greater is not installed or not found in PATH.";
        std::cerr << message << std::endl;
        ShowMessageBox("Error", message);
    }
}

bool IsJavaVersionCorrect() {
    std::string command = "java -version 2>&1";
    std::string result = ExecuteCommandAndGetOutput(command);

    std::regex version_pattern(R"(version "([0-9]+)\.)");
    std::smatch matches;

    if (std::regex_search(result, matches, version_pattern)) {
        if (matches.size() > 1) {
            int version = std::stoi(matches[1]);
            std::cout << "Java version found: " << matches[1] << std::endl;
            return version >= MINIMUM_JAVA_MAJOR_VERSION_REQUIRED;
        }
    }

    return false;
}

std::string ExecuteCommandAndGetOutput(const std::string& command) {
    std::string result;
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    HANDLE hStdOutRead = NULL;
    HANDLE hStdOutWrite = NULL;

    // Set up the security attributes struct.
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    // Create the child output pipe.
    if (!CreatePipe(&hStdOutRead, &hStdOutWrite, &sa, 0)) {
        std::cerr << "CreatePipe failed (" << GetLastError() << ").\n";
        return "";
    }

    // Ensure the read handle to the pipe for STDOUT is not inherited.
    SetHandleInformation(hStdOutRead, HANDLE_FLAG_INHERIT, 0);

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    si.wShowWindow = SW_HIDE; // Prevent console window from showing
    si.hStdOutput = hStdOutWrite;
    si.hStdError = hStdOutWrite;

    ZeroMemory(&pi, sizeof(pi));

    // Create the process
    if (!CreateProcessA(NULL,   // No module name (use command line)
                        (LPSTR)command.c_str(), // Command line
                        NULL,                   // Process handle not inheritable
                        NULL,                   // Thread handle not inheritable
                        TRUE,                   // Set handle inheritance to TRUE
                        CREATE_NO_WINDOW,       // No window is created
                        NULL,                   // Use parent's environment block
                        NULL,                   // Use parent's starting directory
                        &si,                    // Pointer to STARTUPINFO structure
                        &pi)                    // Pointer to PROCESS_INFORMATION structure
            ) {
        std::cerr << "CreateProcess failed (" << GetLastError() << ").\n";
        CloseHandle(hStdOutWrite);
        CloseHandle(hStdOutRead);
        return "";
    }

    // Close the write end of the pipe before reading from the read end of the pipe.
    CloseHandle(hStdOutWrite);

    // Read output from the child process
    DWORD read;
    CHAR buffer[256];
    while (true) {
        if (!ReadFile(hStdOutRead, buffer, sizeof(buffer) - 1, &read, NULL) || read == 0) break;
        buffer[read] = '\0';
        result += buffer;
    }

    // Close the handles
    CloseHandle(hStdOutRead);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return result;
}

