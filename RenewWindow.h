#pragma once
#include <windows.h>

namespace RenewEngine {
	class RenewWindow
	{
	public:
		RenewWindow(HINSTANCE hInstance, LPCWSTR windowTitle, unsigned int width, unsigned int height);
		void PeekMessages();
		void Show();
		HWND& GetHwnd();
		boolean shouldQuit;
		
	private:
		MSG m_msg;
		HWND m_hwnd;
	};
}


