#pragma once

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include "Framework/Common/BaseApplication.hpp"

namespace Aurora
{
	class WindowsApplication : public BaseApplication
	{
	public:
		static LRESULT CALLBACK WindowProc(HWND hWnd,
			UINT message,
			WPARAM wParam,
			LPARAM lParam);

		WindowsApplication(GfxConfiguration& cfg);
		~WindowsApplication();

		virtual bool Initialize();
		virtual void Finalize();
		virtual void Tick();

		HWND GetMainWindow() { return hwnd_; }
	private:
		HWND hwnd_;
	};
}

