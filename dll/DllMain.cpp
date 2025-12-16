#include <Windows.h>
#include <iostream>
#include "GCheat.h"
#include "CrashLogs.h"
static void CreateConsoleWindow() {
    if (AllocConsole()) {
        FILE* file;
        freopen_s(&file, "CONOUT$", "w", stdout);
        freopen_s(&file, "CONOUT$", "w", stderr);
        freopen_s(&file, "CONIN$", "r", stdin);
        std::cout << "Console initialized. Standard output redirected." << std::endl;
    }
}



void* Dll = nullptr;
void ClearingThread() {
    glaiel::crashlogs::set_crashlog_folder("c:/.xanax/crash_logs/bl3");
    glaiel::crashlogs::begin_monitoring();

     try {
            MH_Initialize();
            g_cheat->init();
        }catch (const std::exception &e) {
            g_cheat->title =  e.what();
            MessageBoxA(nullptr , e.what() , nullptr , 0);
        }


    while (true) {
        Sleep(20);
        if (GetAsyncKeyState(VK_DELETE) & 1) {
            std::cout <<"Free.\n";
            g_cheat->remove();
            FreeLibraryAndExitThread((HMODULE)Dll, 0);
        }
    }

}


BOOL WINAPI DllMain(HINSTANCE dll, DWORD reason, LPVOID reserved)
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
        {
#ifdef _DEBUG
            CreateConsoleWindow();
#endif
            DisableThreadLibraryCalls(dll);
            Dll = dll;
            CreateThread(nullptr, 0, [](void*)->DWORD {
                ClearingThread();
                return 0;

            }, nullptr, 0, nullptr);

            return TRUE;
        }
        case DLL_PROCESS_DETACH:
        {
            g_cheat->remove();
            return true;
        }
    }
    return TRUE;
}