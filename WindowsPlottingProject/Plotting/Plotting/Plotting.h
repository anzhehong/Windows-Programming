#pragma once

#include <Windows.h>
#include "tool.h"

class Plottdoubleing
{
private:

	DWORD defaultStyle = WS_CHILD | WS_VISIBLE | WS_BORDER;
	HWND parentWindow;
	HINSTANCE instance = NULL;
	LPVOID lpParam = NULL;
	HPEN pen;
	HDC hdc;
	HDC hMem;
	HBITMAP hBmp;
	PAINTSTRUCT ps;
	BOOLEAN isClick = false;
	POINT pt;
	POINT oldPt;
	// 坐标原点
	POINT zeroPoint;
	int nowInverval = GLOBAL_INTERVAL;
	// 统一的坐标范围
	POINT range;
	int backGrountColor = WHITE_COLOR;
	bool showTickAndNumber;

public:

	Graph graph;

	/*
	 * 实例化
	 */
	Plottdoubleing(int paintWidth, int paintHeight)
	{
		pen = CreatePen(PS_SOLID, 2, RED_COLOR);
		zeroPoint.x = ( (paintWidth / 2) + WINDOW_INITIAL_X);
		zeroPoint.y = ( (paintHeight / 2) + WINDOW_INITIAL_Y);
		range = {-20, 20};
		graph = Graph();
		showTickAndNumber = false;
	}

	/*
	 *	设置父窗口
	 */
	Plottdoubleing* setParentWindow(HWND parentWnd)
	{
		this->parentWindow = parentWnd;
		this->hdc = GetDC(parentWnd);
		return this;
	}
	/*
		拿到这个窗口
	*/
	HWND getChildWindow()
	{
		return this->parentWindow;
	}

	/*
		设置背景颜色
	*/
	Plottdoubleing* setBackgroundColor(int newColor, HWND hwnd)
	{
		static RECT rect;
		PAINTSTRUCT ps;
		//	myPlotting->setBackgroundColor(YELLOW_COLOR);

		HDC hdc = BeginPaint(hwnd, &ps);
		HBRUSH hBrush = CreateSolidBrush(newColor);
		GetClientRect(hwnd, &rect);
		HDC hDCMem = GetDC(hwnd);
		FillRect(hDCMem, &rect, hBrush);  //用画刷把位图清除干净

		BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hDCMem, 0, 0, SRCCOPY);
		DeleteObject(hBrush);
		EndPaint(hwnd, &ps);
		this->backGrountColor = newColor;
		this->DrawAssit(parentWindow);
		return this;
	}
	int getBackgroundColor()
	{
		return this->backGrountColor;
	}

	/*
	*	绘制坐标系
	*/
	Plottdoubleing* setShowTickAndNumber()
	{
		if (this->showTickAndNumber)
			showTickAndNumber = false;
		else
			showTickAndNumber = true;
		return this;
	}
	bool getShowTickAndNumber()
	{
		return this->showTickAndNumber;
	}

	Plottdoubleing* DrawAssit(HWND hwnd)
	{
		//PAINTSTRUCT ps;
		//HDC hdc;   // DC(可画图的内存对象) 的句柄
		//HPEN hpen; // 画笔
		//int x, y;
		//double i;

		//hdc = BeginPaint(hwnd, &ps);
	//	hpen = CreatePen(PS_DOT, 1, GRAY_COLOR);
	//	SelectObject(hdc, hpen);

		//调用tool.h中绘制网格的接口
		createGrid(GetDC(hwnd), zeroPoint, nowInverval);
		if (showTickAndNumber)
			createTickAndNumber(GetDC(hwnd), zeroPoint, nowInverval);
		/*OutputDebugString(ToString(zeroPoint.x).c_str());
		OutputDebugString(",");
		OutputDebugString(ToString(zeroPoint.y).c_str());
		OutputDebugString("\n");*/

	//	DeleteObject(hpen);
		return this;
		
	}

	/*
	 *	以下是移动坐标轴的方法
	 */
	//	isClick的get方法
	BOOLEAN getClick() 
	{
		return this->isClick;
	}
	//	在移动鼠标的过程中
	Plottdoubleing* Moving(POINT oldPoint)
	{
		isClick = true;
		this->oldPt = oldPoint;
		return this;
	}

	Plottdoubleing* changeRange(POINT newRange)
	{
		this->range = newRange;
		return this;
	}
	POINT getRange()
	{
		return this->range;
	}

	//	在移动鼠标之后
	Plottdoubleing* Moved() 
	{
		isClick = false;
		return this;
	}

	//	拖动
	
	Plottdoubleing* dragFunction(POINT point)
	{
		//	改变原点
		this->zeroPoint.x = point.x + this->zeroPoint.x - oldPt.x;
		this->zeroPoint.y = point.y + this->zeroPoint.y - oldPt.y;  
		drawARectangle(hdc, { 0,0 }, { (WINDOW_INITIAL_X + WINDOW_WIDTH), (WINDOW_INITIAL_Y + WINDOW_HEIGHT) }, backGrountColor);
		PostMessage(parentWindow, WM_PAINT, 0, 0);
		oldPt = point;
		return this;
	}
	Plottdoubleing* cleanBoard()
	{
		drawARectangle(hdc, { 0,0 }, { (WINDOW_INITIAL_X + WINDOW_WIDTH), (WINDOW_INITIAL_Y + WINDOW_HEIGHT) }, backGrountColor);
		return this;
	}

	Plottdoubleing* dragMovingFunction(POINT point)
	{
		//	改变原点
		this->zeroPoint.x = point.x + this->zeroPoint.x - oldPt.x;
		this->zeroPoint.y = point.y + this->zeroPoint.y - oldPt.y;
		//drawARectangle(hdc, { WINDOW_INITIAL_X,0 }, { (WINDOW_INITIAL_X + WINDOW_WIDTH), (WINDOW_INITIAL_Y + WINDOW_HEIGHT) }, WHITE_COLOR, WHITE_COLOR);
		
		return this;
	}
	
	Plottdoubleing* changeNowInterval(int change)
	{
		this->nowInverval += change;
		if (this->nowInverval > 100)
		{
			this->nowInverval = 100;
			return this;
		}if (this->nowInverval < 20)
		{
			this->nowInverval = 20;
			return this;
		}


		drawARectangle(hdc, { -WINDOW_INITIAL_X,-WINDOW_INITIAL_Y }, { (WINDOW_INITIAL_X + WINDOW_WIDTH), (WINDOW_INITIAL_Y + WINDOW_HEIGHT) }, backGrountColor);
		PostMessage(parentWindow, WM_PAINT, 0, 0);
		return this;
	}


	/*
	 *	绘制函数图像
	 */
	Plottdoubleing* drawAGraph(int editId, int color, POINT rage, HWND editContainer)
	{
		string mathExpression = getWindowText(GetDlgItem(editContainer, editId));
		if (mathExpression == "")
			return this;
		DoulePoint originData = positionConvert(range.x, -calculate1(range.x, mathExpression, mathExpression.size() + 1));
		DoulePoint nextData = { 0, 0 };
		double length = range.y - range.x;
		for (double i = 0.01; i < length; )
		{
			double x = range.x + i;
			double y = -calculate1(x, mathExpression, mathExpression.size() + 1);
			nextData = positionConvert(x, y);
			POINT originGraph = ConvertDoublePointToPoint(originData);
			POINT nextGraph = ConvertDoublePointToPoint(nextData);
			drawALine(hdc, originGraph, nextGraph, color);
		//	drawASmoothLine(hdc, originGraph, nextGraph, color);
			i += DEFAULT_SPLITCOUNT;
			originData = nextData;
		}

		return this;
	}


	/*
		坐标转换
	*/
	DoulePoint positionConvert(double xValue, double yValue) {
		double proportion = nowInverval * 1.0 / GLOBAL_INTERVALValue;
		DoulePoint doublePoint = { (xValue - 0) * proportion + zeroPoint.x , (yValue - 0) * proportion + zeroPoint.y };
		return doublePoint;
	}
	POINT ConvertDoublePointToPoint(DoulePoint dp) {
		POINT result = { int(dp.x), int(dp.y) };
		return{ int(dp.x), int(dp.y) };
	}
};