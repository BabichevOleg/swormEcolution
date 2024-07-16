#include "Logger.h"
#include "Evolution.h"

#include <Windows.h>

int main() {
    for (size_t n = 1; n <= 100; ++n) {

        main_log("../data/log.csv", n);

        std::vector<PROCESS_INFORMATION> processInfos;

        HANDLE semaphore = CreateSemaphore(NULL, 1, 1, "SemaphoreToGetValue");

        for (size_t i = 0; i < 20; ++i) {
            _STARTUPINFOA si;
            PROCESS_INFORMATION pi;
            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            ZeroMemory(&pi, sizeof(pi));

            if (!CreateProcess(NULL,
                "..\\build\\Release\\client.exe",
                NULL,
                NULL,
                FALSE,
                0,
                NULL,
                NULL,
                &si,
                &pi))
            {
                std::cerr << "CreateProcess failed: " << GetLastError() << std::endl;
            }
            processInfos.push_back(pi);
        }

        for (const auto& processInfo : processInfos) {
            // ќжидание завершени€ процесса
            WaitForSingleObject(processInfo.hProcess, INFINITE);
        }
        for (const auto& processInfo : processInfos) {
            CloseHandle(processInfo.hProcess);
            CloseHandle(processInfo.hThread);
        }

        CloseHandle(semaphore);

        evolution();
    }

    return 0;
}