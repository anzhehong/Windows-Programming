#include <windows.h>
#include "stdafx.h"
#include <math.h>

#define POS_X 500	//坐标原点X
#define POS_Y 500	//坐标原点Y
#define POS_WIDTH 1000	//坐标系宽度

/*颜色*/
#define BLACK_COLOR RGB(0, 0, 0)	//黑色
#define ReadCorlor RGB(255, 0, 0)	//红色

POINTS ptOld;
HDC hMem = NULL;

/*
	要绘制的函数：
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
	一个画直线的函数
*/
void PaintSin(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc;   // DC(可画图的内存对象) 的句柄
	HPEN hpen; // 画笔
	int x, y;
	double i;

	hdc = BeginPaint(hwnd, &ps);
	hpen = CreatePen(PS_SOLID, 1, BLACK_COLOR);
	SelectObject(hdc, hpen);

	// 绘制 x 轴
	MoveToEx(hdc, POS_X - POS_WIDTH, POS_Y, NULL);
	LineTo(hdc, POS_X + POS_WIDTH, POS_Y);

	// 绘制 y 轴
	MoveToEx(hdc, POS_X, POS_Y + POS_WIDTH, NULL);
	LineTo(hdc, POS_X, POS_Y - POS_WIDTH);

	// 绘制 f(x) = sin(x/10) * 20 的函数图像
	for (i = -POS_WIDTH; i < POS_WIDTH; i += 0.05) // x每增长0.05绘制1个点
	{
		x = POS_X + i;
		y = POS_Y + f(i);
		// 在 (x,y) 处绘制一个点
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
	HDC hdc;   // DC(可画图的内存对象) 的句柄
	HPEN hpen; // 画笔
	int red, blue; // 红、绿两色
	int y = 1, x = 1; // 行、列
	// 
	// 通过窗口句柄获取该窗口的 DC
	hdc = BeginPaint(hwnd, &ps);

	for (red = 0; red < 256; red++)
	{
		for (blue = 0; blue < 256; blue++)
		{
			// 创建画笔
			hpen = CreatePen(PS_SOLID, 1, RGB(red, 0, blue));
			// 选中画笔
			SelectObject(hdc, hpen);

			// 画一条从 (x, y) 到 (x, y+1) 的垂直的线
			MoveToEx(hdc, x, y, NULL);
			LineTo(hdc, x++, y + 1);

			// 删除画笔
			DeleteObject(hpen);
		}
		// 一行画完 行+1, 列重新置 1
		y += 1;
		x = 1;
	}

	// 关闭DC 结束绘制
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

	//创建窗口
	hwnd = CreateWindow(L"HELLO", L"Windows Plotting Project by Fowafolo", WS_OVERLAPPEDWINDOW, 10, 10,
		1000, 1000, NULL, NULL, hInstance, NULL);

	::ShowWindow(hwnd, SW_SHOWNORMAL);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		DispatchMessage(&msg);
	}

	return 0;
}