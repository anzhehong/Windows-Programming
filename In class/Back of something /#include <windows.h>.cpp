#include <windows.h>
#include "stdafx.h"

POINTS ptOld;
HDC hMem = NULL;

LRESULT _stdcall MyWinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (Msg)
	{
	
	case WM_MOUSEMOVE:
	{
		/*
		POINTS  pt = MAKEPOINTS(lParam);
		HDC hdc = GetDC(hWnd);
		MoveToEx(hdc, ptOld.x, ptOld.y, NULL);
		LineTo(hdc, pt.x, pt.y);
		::ReleaseDC(hWnd, hdc);
		ptOld = pt;
		*/
		
	}
	break;

	case WM_LBUTTONDOWN:
	{
		HDC hdc = GetDC(hWnd);
		hMem = ::CreateCompatibleDC(hdc);

		HBITMAP hBmp = ::CreateCompatibleBitmap(hdc, 500, 500);
		::SelectObject(hMem, hBmp);

		::ReleaseDC(hWnd, hdc);

		HPEN hpen = ::CreatePen(PS_DASH, 5, RGB(255, 0, 0));
		HPEN hpenOld = (HPEN)::SelectObject(hMem, hpen);

		HBRUSH hbr = ::CreateSolidBrush(RGB(120, 0, 111));
		HBRUSH hbrOld = (HBRUSH)::SelectObject(hMem, hbr);

		MoveToEx(hMem, 300, 300, NULL);
		LineTo(hMem, 300, 300);

		Rectangle(hMem, 10, 10, 90, 90);

		TextOut(hMem, 100, 100, L"hello world", 11);

		SelectObject(hMem, hpenOld);
		SelectObject(hMem, hbrOld);

		::DeleteObject(hpen);
		::DeleteObject(hbr);
	}
	::InvalidateRect(hWnd, NULL, TRUE);

	break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		//黑黑的窗口
		BitBlt(hdc, 0, 0, 500, 500, hMem, 0, 0, SRCCOPY);
		BitBlt(hdc, 500, 500, 500, 500, hMem, 0, 0, SRCAND);
		
		HPEN hpen = ::CreatePen(PS_DASH, 5, RGB(255, 0, 0));
		HPEN hpenOld = (HPEN)::SelectObject(hdc, hpen);

		HBRUSH hbr = ::CreateSolidBrush(RGB(0, 0, 255));
		HBRUSH hbrOld =(HBRUSH)::SelectObject(hdc, hbr);

		MoveToEx(hdc, 100, 100, NULL);
		LineTo(hdc, 100, 100);

		Rectangle(hdc, 10, 10, 90, 90);

		TextOut(hdc, 100, 100, L"hello mike", 10);

		SelectObject(hdc, hpenOld);
		SelectObject(hdc, hbrOld);

		::DeleteObject(hpen);
		::DeleteObject(hbr);









		
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return ::DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	return 0;
}

int APIENTRY wWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	WNDCLASSEX wc;
	ATOM atom;
	HWND hwnd;
	MSG msg;

	::ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"HELLO";
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpfnWndProc = MyWinProc;

	atom = ::RegisterClassEx(&wc);

	hwnd = CreateWindow(L"HELLO", L"WindowName", WS_OVERLAPPEDWINDOW, 10, 10,
		1000, 1000, NULL, NULL, hInstance, NULL);

	::ShowWindow(hwnd, SW_SHOWNORMAL);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		DispatchMessage(&msg);
	}

	return 0;
}