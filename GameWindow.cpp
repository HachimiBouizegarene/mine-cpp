#include "RenewWindow.h"
#include "Helper.h"
#include <exception>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


RenewEngine::RenewWindow::RenewWindow(HINSTANCE hInstance, LPCWSTR windowTitle, unsigned int width, unsigned int height)
{
	LPCWSTR pClassName = L"RenewEngineWindowClass";
	
	WNDCLASSEX windowClass = {};

	windowClass.hInstance = hInstance;
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpszClassName = pClassName;
	windowClass.lpfnWndProc = WindowProc;

	ATOM atom = RegisterClassExW(&windowClass);
	if (atom <= 0) throw std::exception();

	m_hwnd = CreateWindowExW(NULL, pClassName, windowTitle, WS_POPUP, 0, 0, width, height, NULL, NULL, hInstance, nullptr);

	if (!m_hwnd) throw std::exception();

	Show();
}

void RenewEngine::RenewWindow::PeekMessages()
{
	if (PeekMessageW(&m_msg, NULL, NULL, NULL, PM_REMOVE)) {
		if (m_msg.message == WM_QUIT)
		{
			shouldQuit = true;
		}
		TranslateMessage(&m_msg);
		DispatchMessage(&m_msg);
	}
}

void RenewEngine::RenewWindow::Show()
{
	ShowWindow(m_hwnd, SW_SHOW);
}

HWND& RenewEngine::RenewWindow::GetHwnd()
{
	return m_hwnd;
}
