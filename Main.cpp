#include <windows.h>              // Core Windows types and functions
#include <unordered_map>          // Hash map (like Python dict)
#include <iostream>               // Console output
#include "MultiMouseTracker.h"


// Forward Declaration for WindowProc (this function exists but I'll define it later)
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Stores the accumulated position for each mouse
// Key = device handle (unique per mouse)
// Value = POINT struct holding x/y position
std::unordered_map<HANDLE, POINT> mousePositions;

MultiMouseTracker g_mouseTracker;

// ----------------------ADDING ENTRY POINT FOR WINDOWS-----------------------------------------------------------------------
int WINAPI WinMain(
	HINSTANCE hInstance,          // Handle to this program instance
	HINSTANCE,                    // Unused (legacy)
	LPSTR,                        // Command line arguments
	int nCmdShow                  // How the window should be shown
) {
	// Create a console for debugging output
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);

	// Define a window class (template for windows)
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;  // Our message handler
	wc.hInstance = hInstance;
	wc.lpszClassName = L"RawInputTestWindow";

	// Register the window class with Windows
	RegisterClass(&wc);

	// Create an actual window from the class
	HWND hwnd = CreateWindowEx(
		0,                        // Optional styles
		wc.lpszClassName,         // Class name
		L"Raw Input Test",        // Window title
		WS_OVERLAPPEDWINDOW,      // Window style
		CW_USEDEFAULT, CW_USEDEFAULT,  // Position
		800, 600,                 // Size
		nullptr,                  // Parent window
		nullptr,                  // Menu
		hInstance,                // Instance handle
		nullptr                   // Additional data
	);

	// Show the window
	ShowWindow(hwnd, nCmdShow);

	// Register to recieve raw mouse input
	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01;        // Generic desktop controls
	rid.usUsage = 0x02;            // Mouse
	rid.dwFlags = RIDEV_INPUTSINK;
	rid.hwndTarget = hwnd;

	RegisterRawInputDevices(&rid, 1, sizeof(rid));

	// Main message loop (this keeps the app alive)
	MSG msg = {};
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
// ---------------------------------------------------------------------------------------------------------------------------


// This function is called by windows whenever something happens
LRESULT CALLBACK WindowProc(
	HWND hwnd,                    // Window recieving the message
	UINT msg,                     // Message type (what happened)
	WPARAM wParam,                // Extra data (depends on msg)
	LPARAM lParam)                // Extra data (depends on msg)
{
	// Check if this message contains raw input data
	if (msg == WM_INPUT) {
		g_mouseTracker.processRawInput(lParam);
	}

	// Let Windows handle anything that didn't process (NEED this or Windows will break)
	return DefWindowProc(hwnd, msg, wParam, lParam);
}