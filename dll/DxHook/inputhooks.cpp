#include "pch.h"
#include "namespaces.h"
#include "imgui/imgui_impl_win32.h"
#include "GUI/menu/MainGui.hpp"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace inputhook {
    static WNDPROC sOriginalWndProc = nullptr;

    // Cache the window handle to allow later removal of the hook
    // (stored in globals::mainWindow for cross-namespace access)

    void Init(HWND hWindow)
    {
        //DebugLog("[inputhook] Initializing input hook for window %p\n", hWindow);

        // Store window globally for later use during release
        globals::mainWindow = hWindow;

        sOriginalWndProc = (WNDPROC)SetWindowLongPtr(hWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);
        if (!sOriginalWndProc) {
            //DebugLog("[inputhook] Failed to set WndProc: %d\n", GetLastError());
        }
        else {
            //DebugLog("[inputhook] WndProc hook set. Original WndProc=%p\n", sOriginalWndProc);
        }
    }

    void Remove(HWND hWindow)
    {
        if (!sOriginalWndProc) {
            //DebugLog("[inputhook] WndProc hook already removed or was never set\n");
            return;
        }

        //DebugLog("[inputhook] Removing input hook for window %p\n", hWindow);
        if (SetWindowLongPtr(hWindow, GWLP_WNDPROC, (LONG_PTR)sOriginalWndProc) == 0) {
            //DebugLog("[inputhook] Failed to restore WndProc: %d\n", GetLastError());
        }
        else {
            //DebugLog("[inputhook] WndProc restored to %p\n", sOriginalWndProc);
        }

        // Clear cached values to prevent repeated removals
        sOriginalWndProc = nullptr;
        globals::mainWindow = nullptr;
    }

    LRESULT APIENTRY WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {

        LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

        //TODO 如果窗口有问题 直接 这里 false
        if (MainMenu::showMenu) {
            switch (uMsg) {
                case WM_MOUSEMOVE:
                case WM_NCMOUSEMOVE:
                case WM_MOUSELEAVE:
                case WM_NCMOUSELEAVE:
                case WM_LBUTTONDOWN:
                case WM_LBUTTONDBLCLK:
                case WM_RBUTTONDOWN:
                case WM_RBUTTONDBLCLK:
                case WM_MBUTTONDOWN:
                case WM_MBUTTONDBLCLK:
                case WM_XBUTTONDOWN:
                case WM_XBUTTONDBLCLK:
                case WM_LBUTTONUP:
                case WM_RBUTTONUP:
                case WM_MBUTTONUP:
                case WM_XBUTTONUP:
                case WM_MOUSEWHEEL:
                case WM_MOUSEHWHEEL:
                case WM_KEYDOWN:
                case WM_KEYUP:
                case WM_SYSKEYDOWN:
                case WM_SYSKEYUP:
                case WM_SETFOCUS:
                case WM_KILLFOCUS:
                case WM_CHAR:
                case WM_SETCURSOR:
                case WM_DEVICECHANGE:
                    return true;
            }

            return true;
        }

        return CallWindowProcA(sOriginalWndProc, hWnd, uMsg, wParam, lParam);
    }
}
