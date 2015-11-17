#include <windows.h>
#include "stdafx.h"
#include <math.h>

#define POS_X 500	//����ԭ��X
#define POS_Y 500	//����ԭ��Y
#define POS_WIDTH 1000	//����ϵ���

/*��ɫ*/
#define BLACK_COLOR RGB(0, 0, 0)	//��ɫ
#define ReadCorlor RGB(255, 0, 0)	//��ɫ

POINTS ptOld;
HDC hMem = NULL;

/*
	Ҫ���Ƶĺ�����
	f(x) = sin(x/10) * 20
*/
double f(double x)
{
	return sin(x / 10) * 20;
}

double fx(double x)
{
	return x;
}


/*
	һ����ֱ�ߵĺ���
*/
void PaintSin(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc;   // DC(�ɻ�ͼ���ڴ����) �ľ��
	HPEN hpen; // ����
	int x, y;
	double i;

	hdc = BeginPaint(hwnd, &ps);
	hpen = CreatePen(PS_SOLID, 1, BLACK_COLOR);
	SelectObject(hdc, hpen);

	// ���� x ��
	MoveToEx(hdc, POS_X - POS_WIDTH, POS_Y, NULL);
	LineTo(hdc, POS_X + POS_WIDTH, POS_Y);

	// ���� y ��
	MoveToEx(hdc, POS_X, POS_Y + POS_WIDTH, NULL);
	LineTo(hdc, POS_X, POS_Y - POS_WIDTH);

	// ���� f(x) = sin(x/10) * 20 �ĺ���ͼ��
	for (i = -POS_WIDTH; i < POS_WIDTH; i += 0.05) // xÿ����0.05����1����
	{
		x = POS_X + i;
		y = POS_Y + f(i);
		// �� (x,y) ������һ����
		SetPixel(hdc, x, y, BLACK_COLOR);
	}

	DeleteObject(hpen);
	EndPaint(hwnd, &ps);
}
void PaintX(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc;
	HPEN hpen;
	int x, y;
	double i;
	hdc = BeginPaint(hwnd, &ps);
	hpen = CreatePen(PS_SOLID, 1, ReadCorlor);
	SelectObject(hdc, hpen);

	for (i = -POS_WIDTH; i < POS_WIDTH; i += 0.5)
	{
		x = POS_X + i;
		y = POS_Y + fx(i);
		SetPixel(hdc, x, y, ReadCorlor);
	}

	DeleteObject(hpen);
	EndPaint(hwnd, &ps);
}




void Paint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc;   // DC(�ɻ�ͼ���ڴ����) �ľ��
	HPEN hpen; // ����
	int red, blue; // �졢����ɫ
	int y = 1, x = 1; // �С���
	// 
	// ͨ�����ھ����ȡ�ô��ڵ� DC
	hdc = BeginPaint(hwnd, &ps);

	for (red = 0; red < 256; red++)
	{
		for (blue = 0; blue < 256; blue++)
		{
			// ��������
			hpen = CreatePen(PS_SOLID, 1, RGB(red, 0, blue));
			// ѡ�л���
			SelectObject(hdc, hpen);

			// ��һ���� (x, y) �� (x, y+1) �Ĵ�ֱ����
			MoveToEx(hdc, x, y, NULL);
			LineTo(hdc, x++, y + 1);

			// ɾ������
			DeleteObject(hpen);
		}
		// һ�л��� ��+1, �������� 1
		y += 1;
		x = 1;
	}

	// �ر�DC ��������
	EndPaint(hwnd, &ps);
}

LRESULT _stdcall MyWinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (Msg)
	{
	
	case WM_MOUSEMOVE:
	{
		
	}
	break;

	case WM_LBUTTONDOWN:
	{

	}
	::InvalidateRect(hWnd, NULL, TRUE);

	break;
	case WM_PAINT:
	{
		PaintSin(hWnd);
		PaintX(hWnd);

		//TODO: make complex plotted graph
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


/*
	Main Function in Windows
*/
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

	//��������
	hwnd = CreateWindow(L"HELLO", L"Windows Plotting Project by Fowafolo", WS_OVERLAPPEDWINDOW, 10, 10,
		1000, 1000, NULL, NULL, hInstance, NULL);

	::ShowWindow(hwnd, SW_SHOWNORMAL);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		DispatchMessage(&msg);
	}

	return 0;
}