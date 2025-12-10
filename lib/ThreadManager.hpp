#pragma once
#include <vector>
#include <Windows.h>
#include <tlhelp32.h>
class ThreadSuspender {
private:
    std::vector<DWORD> suspendedThreads;
    DWORD currentProcessId;

public:
    ThreadSuspender() : currentProcessId(GetCurrentProcessId()) {}

    bool SuspendOtherThreads() {
        DWORD currentThreadId = GetCurrentThreadId();
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE) {
            return false;
        }

        THREADENTRY32 te;
        te.dwSize = sizeof(THREADENTRY32);

        if (Thread32First(hSnapshot, &te)) {
            do {
                if (te.th32OwnerProcessID == currentProcessId &&
                    te.th32ThreadID != currentThreadId) {

                    HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te.th32ThreadID);
                    if (hThread) {
                        if (SuspendThread(hThread) != (DWORD)-1) {
                            suspendedThreads.push_back(te.th32ThreadID);
                        }
                        CloseHandle(hThread);
                    }
                    }
            } while (Thread32Next(hSnapshot, &te));
        }

        CloseHandle(hSnapshot);
        return true;
    }

    void ResumeAll() {
        for (DWORD threadId : suspendedThreads) {
            HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, threadId);
            if (hThread) {
                ResumeThread(hThread);
                CloseHandle(hThread);
            }
        }
        suspendedThreads.clear();
    }

    ~ThreadSuspender() {
        ResumeAll();
    }

    // 获取挂起的线程数量
    size_t GetSuspendedCount() const {
        return suspendedThreads.size();
    }
};