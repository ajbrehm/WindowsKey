#include <Windows.h>

HHOOK g_hKeyboardHook;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode < 0 || nCode != HC_ACTION)  // do not process message 
        return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);

    BOOL bEatKeystroke = FALSE;
    KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;
    switch (wParam) {
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            bEatKeystroke = (p->vkCode == VK_LWIN) || (p->vkCode == VK_RWIN);
            break;
        }
    }

    if (bEatKeystroke)
        return 1;
    else
        return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Initialization
    g_hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0);

    // Wait for user to allow reactivation of the Windows key
    MessageBox(NULL, L"Click OK to exit the program and reactivate the Windows key.", L"WindowsKey", 0);

    // Cleanup before shutdown
    UnhookWindowsHookEx(g_hKeyboardHook);

    return 0;
}

