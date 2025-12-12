#include "flashing_window.h"
#include <windows.h>

// Standard console/normal entry point.
// This calls the RunFlashingWindow function which creates the window and runs the message loop.
int main() {
    // Call the window runner with the current module handle and default show state.
    return RunFlashingWindow(GetModuleHandle(NULL), SW_SHOWDEFAULT);
}