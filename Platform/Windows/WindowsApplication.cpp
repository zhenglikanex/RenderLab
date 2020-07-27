#include "WindowsApplication.hpp"

#include "Framework/Utils/StringUtils.hpp"

using namespace Aurora;

extern IApplication* g_app;

WindowsApplication::WindowsApplication(GfxConfiguration& cfg)
	:BaseApplication(cfg)
{

}

WindowsApplication::~WindowsApplication()
{

}

bool WindowsApplication::Initialize()
{
	if (!BaseApplication::Initialize())
	{
		return false;
	}

	// get the HINSTANCE of the Console Program
	HINSTANCE hInstance = GetModuleHandle(NULL);

	// the handle for the window, filled by a function
	HWND hWnd;
	// this struct holds information for the window class
	WNDCLASSEX wc;

	// clear out the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = _T("MyEngine");

	// register the window class
	RegisterClassEx(&wc);

	// create the window and use the result as the handle
	hWnd = CreateWindowExW(0,
		L"MyEngine",      // name of the window class
		StringUtils::String2WString(config_.app_name).c_str(),             // title of the window
		WS_OVERLAPPEDWINDOW,              // window style
		CW_USEDEFAULT,                    // x-position of the window
		CW_USEDEFAULT,                    // y-position of the window
		config_.screen_width,             // width of the window
		config_.screen_height,            // height of the window
		NULL,                             // we have no parent window, NULL
		NULL,                             // we aren't using menus, NULL
		hInstance,                        // application handle
		NULL);                            // used with multiple windows, NULL

// display the window on the screen
	ShowWindow(hWnd, SW_SHOW);

	hwnd_ = hWnd;

	return true;
}

void WindowsApplication::Finalize()
{

}

void WindowsApplication::Tick()
{
	// this struct holds Windows event messages
	MSG msg;

	// we use PeekMessage instead of GetMessage here
	// because we should not block the thread at anywhere
	// except the engine execution driver module 
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		// translate keystroke messages into the right format
		TranslateMessage(&msg);

		// send the message to the WindowProc function
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK WindowsApplication::WindowProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
	case WM_PAINT:
		// we will replace this part with Rendering Module
	{
	} break;

	// this message is read when the window is closed
	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		g_app->Quit();
		return 0;
	}
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}