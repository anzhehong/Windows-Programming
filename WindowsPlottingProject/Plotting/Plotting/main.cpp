#pragma once



#include "resource.h"
#include "Plotting.h"
#include <cmath>
#include <wchar.h>
#include "minicsv.h"
const double pi = 3.1415926535;

HINSTANCE hInst;	//当前实例
HWND hWndMain;	//主窗口
HWND painHwnd;

// 此代码模块中包含的函数的前向声明: 
BOOL CALLBACK DiaProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
//void DrawAssit(HWND hwnd);
void drawGraphWhileMoving(int editId, int color);
Plottdoubleing* myPlotting = new Plottdoubleing(WINDOW_WIDTH, WINDOW_HEIGHT);
void changeBackground(HWND hwnd);
void parsingColor(string colorExpression, int index);

vector<int> colors = { RED_COLOR,BLUE_COLOR,GREEN_COLOR,YELLOW_COLOR };

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, PSTR lpszCmdLine, int nCmdShow)
{
	static TCHAR szAppName[] = TEXT("HelloWinPlotting");

	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	//注册窗口
	if (!RegisterClass(&wndclass))

	{
		//错误提示
		MessageBox(NULL, TEXT("This program requires Windows NT!"),
			szAppName, MB_ICONERROR);
		return 0;
	}

	MSG msg;

	hWndMain = CreateDialogParam(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DiaProc, NULL);

	//添加图像层
	painHwnd = CreateWindow(szAppName, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, WINDOW_INITIAL_X, WINDOW_INITIAL_Y, WINDOW_WIDTH, WINDOW_HEIGHT, hWndMain, NULL, hInstance, NULL);
	PostMessage(painHwnd, WM_PAINT, NULL, NULL);
	ShowWindow(painHwnd, nCmdShow);
	UpdateWindow(painHwnd);

	ShowWindow(hWndMain, nCmdShow);
	UpdateWindow(hWndMain);
	
	
	//主消息循环
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}



//主窗口回掉函数
BOOL CALLBACK DiaProc(HWND hwnd, UINT message,WPARAM wParam, LPARAM lParam)
{
	static RECT rect;
	PAINTSTRUCT ps;
	



	switch (message) {

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			/*
				函数图像1
			*/
		case IDC_BUTTON1: {
			//MessageBox(hWndMain,"hello", 0, 0);
			//OutputDebugString("das");
		/*	string func1Str = getWindowText(GetDlgItem(hWndMain, IDC_EDIT1));
			myPlotting->drawAGraph(IDC_EDIT1, RED_COLOR, { -10,10 }, hWndMain);*/
			//	drawGraphWhileMoving(IDC_EDIT1, RED_COLOR);
			parsingColor(getWindowText(GetDlgItem(hWndMain, IDC_COMBO1)), 0);
			drawGraphWhileMoving(IDC_EDIT1, colors[0]);
			break;
		}
		/*
			函数图像2
		*/
		case IDC_BUTTON2: {
			parsingColor(getWindowText(GetDlgItem(hWndMain, IDC_COMBO2)), 1);
			drawGraphWhileMoving(IDC_EDIT2, colors[1]);
			break;
		}
		/*
			函数图象3
		*/
		case IDC_BUTTON3: {
			drawGraphWhileMoving(IDC_EDIT3, colors[2]);
			parsingColor(getWindowText(GetDlgItem(hWndMain, IDC_COMBO3)), 2);
			break;
		}
		/*
		函数图像4
		*/
		case IDC_BUTTON4: {
			drawGraphWhileMoving(IDC_EDIT4, colors[3]);
			parsingColor(getWindowText(GetDlgItem(hWndMain, IDC_COMBO4)), 3);
			break;
		}
		/*
			函数图像X区域
		*/
		case IDC_BUTTON5: {
			string fromStr = getWindowText(GetDlgItem(hWndMain, IDC_EDIT5));
			string toStr = getWindowText(GetDlgItem(hWndMain, IDC_EDIT6));
			int fromInt, toInt;
			if (fromStr != "")
				fromInt = stoi(fromStr);
			else
				fromInt = myPlotting->getRange().x;

			if (toStr != "")
				toInt = stoi(toStr);
			else
				toInt = myPlotting->getRange().y;
			
			POINT newRange = {fromInt, toInt};
			myPlotting->changeRange(newRange);
			PostMessage(myPlotting->getChildWindow(), WM_PAINT, 0, 0);
			break;
		}
		/*
			改变背景						  	
		*/
		case IDC_BUTTON7: {
			if (myPlotting->getBackgroundColor() == WHITE_COLOR)
				myPlotting->setBackgroundColor(YELLOW_COLOR, painHwnd);
			else
				myPlotting->setBackgroundColor(WHITE_COLOR, painHwnd);
	
			break;
		}
		/*
			导出图片
		*/
		case IDC_BUTTON6: {
			HDC hDC = GetDC(hwnd);
			HDC hMemDC = CreateCompatibleDC(hDC);
			HBITMAP hBitmap = CreateCompatibleBitmap(hDC, WINDOW_WIDTH, WINDOW_HEIGHT);
			hBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
			BitBlt(hMemDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hDC, 0, 0, SRCCOPY);
			hBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
			CreateBMPFile(hwnd, "Output.bmp", CreateBitmapInfoStruct(hwnd, hBitmap), hBitmap, hMemDC);
			ReleaseDC(hwnd, hDC);
			MessageBox(hwnd, "保存成功", NULL, NULL);
			break;
		}
		/*
		修改tick and number
		*/
		case IDC_BUTTON8: {
			myPlotting->setShowTickAndNumber();
			PostMessage(myPlotting->getChildWindow(), WM_PAINT, 0, 0);
			break;
		}
		/*
			读取CSV
		*/
		case IDC_BUTTON9: {
			//MessageBox(hwnd, "读取成功", NULL, NULL);
			csv::ifstream is("testCSV.csv");
			is.set_delimiter(',', "##");
			string temp;
			vector<string> mathExpression = {};
			int indexTemp = IDC_EDIT1;
			if (is.is_open())
			{
				while (is.read_line())
				{
					for (int i = 0; i < SUPPORTED_FUNCTION_SIZE; i++)
					{
						is >> temp;
						mathExpression.push_back(temp);
						setWindowText(GetDlgItem(hWndMain, indexTemp), temp);
						indexTemp += 2;
					}
					// display the read items
				}
			}
			OutputDebugString(mathExpression[2].c_str());
			MessageBox(hwnd, "读取成功", NULL, NULL);
			PostMessage(myPlotting->getChildWindow(), WM_PAINT, 0, 0);
			break;
		}
		
		default:
			break;
		}
	}
	case WM_PAINT: {
		//changeBackground(hwnd);
	}
	}
	return 0;
}

/*
	解析颜色
*/
void parsingColor(string colorExpression, int index)
{

	//OutputDebugString(colorExpression.c_str());
	switch (colorExpression[0])
	{
		//首字母代表五个颜色的缩写
	case 'R':
		colors[index] = RED_COLOR;
		break;
	case 'B':
		colors[index] = BLUE_COLOR;
		break;
	case 'G':
		colors[index] = GREEN_COLOR;
		break;
	case 'Y':
		colors[index] = YELLOW_COLOR;
		break;
	case 'P':
		colors[index] = PURPLE_COLOR;
		break;
	default:
		colors[index] = RED_COLOR;
		break;
	}

}

void drawGraphWhileMoving(int editId,int color)
{
	int tempIndex = IDC_COMBO1;
	for (int i = 0; i < SUPPORTED_FUNCTION_SIZE; i++)
	{
		myPlotting->drawAGraph(editId, color, { -10, 10 }, hWndMain);
		parsingColor(getWindowText(GetDlgItem(hWndMain, tempIndex)), i);
	//	myPlotting->drawAGraph(editId, color, { -10, 10 }, hWndMain);
		tempIndex++;
	}
	
	
}

/*
	添加颜色的函数
*/
void setColors()
{
	// load the combobox with item list.  
	// Send a CB_ADDSTRING message to load each item
	int tempId = IDC_COMBO1;
	for (int i = 0; i < SUPPORTED_FUNCTION_SIZE; i++)
	{
		HWND hWndComboBox = GetDlgItem(hWndMain, tempId);

		SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Red");
		SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Blue");
		SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Green");
		SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Purple");
		SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Yellow");
	//	SendMessage(hWndComboBox, CB_SETCURSEL, 0, (LPARAM)0);
		SendMessage(hWndComboBox, CB_SETCURSEL, NULL, NULL);
		tempId++;
	}
}
/*
	改变背景颜色
*/
void changeBackground(HWND hwnd)
{
	static RECT rect;
	PAINTSTRUCT ps;
	//	myPlotting->setBackgroundColor(YELLOW_COLOR);

	HDC hdc = BeginPaint(painHwnd, &ps);
	HBRUSH hBrush = CreateSolidBrush(YELLOW_COLOR);
	GetClientRect(hwnd, &rect);
	HDC hDCMem = GetDC(hwnd);
	FillRect(hDCMem, &rect, hBrush);  //用画刷把位图清除干净

	BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hDCMem, 0, 0, SRCCOPY);
	DeleteObject(hBrush);
	EndPaint(hwnd, &ps);
	
}

//画图窗口回调函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
switch (message)
{
case WM_CREATE: 
{
	myPlotting->setParentWindow(hwnd);
	setColors();
}
case WM_PAINT: 
{
//	DrawAssit(hwnd);
	myPlotting->DrawAssit(hwnd);
//	string func4Str = getWindowText(GetDlgItem(hWndMain, IDC_EDIT4));
	//myPlotting->drawAGraph(IDC_EDIT4, YELLOW_COLOR, { -10,10 }, hWndMain);
	//移动中的图像
	//清除现有先
//	myPlotting->cleanBoard();
	
	myPlotting->DrawAssit(hwnd);

	int tempColor = colors[0];
	for (int i = 0; i < SUPPORTED_FUNCTION_SIZE; i++)
	{
		int tempIndex = IDC_EDIT1;
		tempIndex += i * 2;
		switch (i)
		{
		case 1: tempColor = colors[1];
			break;
		case 2: tempColor = colors[2];
			break;
		case 3: tempColor = colors[3];
			break;
		default:
			break;
		}
		drawGraphWhileMoving(tempIndex, tempColor);
	}

	break;
}
case WM_LBUTTONDOWN: 
{
	myPlotting->Moving({ MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y });
	break;
}
case WM_LBUTTONUP:
{
	/*if (myPlotting->getClick())
	{
		POINT changedPoint = { MAKEPOINTS(lParam).x , MAKEPOINTS(lParam).y };
		myPlotting->dragFunction(changedPoint);
	}*/
	myPlotting->Moved();
	break;
}
case WM_MOUSEMOVE: 
{
	if (myPlotting->getClick())
	{
		POINT changedPoint = { MAKEPOINTS(lParam).x , MAKEPOINTS(lParam).y };
		myPlotting->dragFunction(changedPoint);
	}
	/*
		我想在这里实现拖拽移动图像，但是好像不能做到欸
	*/
	break;
}
case WM_DESTROY:
{
	break;
}
case WM_MOUSEWHEEL: 
{
	int change = (short)HIWORD(wParam) / 50;
	myPlotting->changeNowInterval(change);

//	myPlotting->setBackgroundColor(PURPLE_COLOR);
	break;
}
default:
	break;
}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
