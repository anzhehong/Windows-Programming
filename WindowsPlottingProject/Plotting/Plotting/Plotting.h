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
	// ����ԭ��
	POINT zeroPoint;
	int nowInverval = GLOBAL_INTERVAL;
	// ͳһ�����귶Χ
	POINT range;
	int backGrountColor = WHITE_COLOR;
	bool showTickAndNumber;

public:

	Graph graph;

	/*
	 * ʵ����
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
	 *	���ø�����
	 */
	Plottdoubleing* setParentWindow(HWND parentWnd)
	{
		this->parentWindow = parentWnd;
		this->hdc = GetDC(parentWnd);
		return this;
	}
	/*
		�õ��������
	*/
	HWND getChildWindow()
	{
		return this->parentWindow;
	}

	/*
		���ñ�����ɫ
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
		FillRect(hDCMem, &rect, hBrush);  //�û�ˢ��λͼ����ɾ�

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
	*	��������ϵ
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
		//HDC hdc;   // DC(�ɻ�ͼ���ڴ����) �ľ��
		//HPEN hpen; // ����
		//int x, y;
		//double i;

		//hdc = BeginPaint(hwnd, &ps);
	//	hpen = CreatePen(PS_DOT, 1, GRAY_COLOR);
	//	SelectObject(hdc, hpen);

		//����tool.h�л�������Ľӿ�
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
	 *	�������ƶ�������ķ���
	 */
	//	isClick��get����
	BOOLEAN getClick() 
	{
		return this->isClick;
	}
	//	���ƶ����Ĺ�����
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

	//	���ƶ����֮��
	Plottdoubleing* Moved() 
	{
		isClick = false;
		return this;
	}

	//	�϶�
	
	Plottdoubleing* dragFunction(POINT point)
	{
		//	�ı�ԭ��
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
		//	�ı�ԭ��
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
	 *	���ƺ���ͼ��
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
		����ת��
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