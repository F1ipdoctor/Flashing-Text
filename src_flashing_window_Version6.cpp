#include <windows.h>
#include <string>

// Global variable to track text visibility state
static bool isTextVisible = true;
static constexpr int IDT_TIMER1 = 1; // Timer ID
static const std::wstring textToFlash = L"Flashing Text! (Harmless GDI effect)";

// Window Procedure Function (WndProc)
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        SetTimer(hwnd, IDT_TIMER1, 500, NULL);
        return 0;

    case WM_TIMER:
        if (wParam == IDT_TIMER1) {
            isTextVisible = !isTextVisible;
            InvalidateRect(hwnd, NULL, TRUE);
        }
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        if (hdc) {
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            if (isTextVisible) {
                SetTextColor(hdc, RGB(255, 0, 0));
                SetBkMode(hdc, TRANSPARENT);
                DrawTextW(hdc, textToFlash.c_str(), -1, &clientRect,
                          DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }
            EndPaint(hwnd, &ps);
        }
        return 0;
    }

    case WM_DESTROY:
        KillTimer(hwnd, IDT_TIMER1);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcW(hwnd, message, wParam, lParam);
}

// WinMain function (entry point)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"SampleWindowClass";

    WNDCLASSW wc = {};
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClassW(&wc)) {
        MessageBoxW(NULL, L"Failed to register window class", L"Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"GDI Flashing Text Effect",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 200,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) {
        MessageBoxW(NULL, L"Failed to create window", L"Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return static_cast<int>(msg.wParam);
}