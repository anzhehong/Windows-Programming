#pragma once

#include <stdio.h>
#include <windows.h>
#include <windowsx.h>

#include <string>
#include "settings.h"
#include "structure.h"

using namespace std;
node all[200];//后缀表达式栈 

/*
	读取CSV文件
	注意：是从指定目录读取，且CSV文件必须是以逗号为分隔符
	最多读取四个表达式，并将合法的算式打印
*/



/*
	导出文件
*/
void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi,
	HBITMAP hBMP, HDC hDC)
{
	HANDLE hf;                 // file handle  
	BITMAPFILEHEADER hdr;       // bitmap file-header  
	PBITMAPINFOHEADER pbih;     // bitmap info-header  
	LPBYTE lpBits;              // memory pointer  
	DWORD dwTotal;              // total count of bytes  
	DWORD cb;                   // incremental count of bytes  
	BYTE *hp;                   // byte pointer  
	DWORD dwTmp;
	bool judge;
	pbih = (PBITMAPINFOHEADER)pbi;
	lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);
	if (!lpBits) MessageBox(hwnd, "2", "2", NULL);
	//	if (!lpBits)
	//	errhandler("GlobalAlloc", hwnd);

	// Retrieve the color table (RGBQUAD array) and the bits  
	// (array of palette indices) from the DIB.  
	/*	if (!GetDIBits(hDC, hBMP, 0, (WORD)pbih->biHeight, lpBits, pbi,
	DIB_RGB_COLORS))
	{
	errhandler("GetDIBits", hwnd);
	}*/
	judge = GetDIBits(hDC, hBMP, 0, (WORD)pbih->biHeight, lpBits, pbi,
		DIB_RGB_COLORS); if (judge == false) MessageBox(hwnd, "3", "3", NULL);
	// Create the .BMP file.  
	hf = CreateFile(pszFile,
		GENERIC_READ | GENERIC_WRITE,
		(DWORD)0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		(HANDLE)NULL);
	if (hf == INVALID_HANDLE_VALUE) MessageBox(hwnd, "4", "4", NULL);
	//	errhandler("CreateFile", hwnd);
	hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"  
								// Compute the size of the entire file.  
	hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) +
		pbih->biSize + pbih->biClrUsed
		* sizeof(RGBQUAD) + pbih->biSizeImage);
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;

	// Compute the offset to the array of color indices.  
	hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) +
		pbih->biSize + pbih->biClrUsed
		* sizeof(RGBQUAD);

	// Copy the BITMAPFILEHEADER into the .BMP file.  
	/*	if (!WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER),
	(LPDWORD)&dwTmp, NULL))
	{
	errhandler("WriteFile", hwnd);
	}*/
	judge = WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER),
		(LPDWORD)&dwTmp, NULL); if (judge == false) MessageBox(hwnd, "5", "5", NULL);
	// Copy the BITMAPINFOHEADER and RGBQUAD array into the file.  
	/*	if (!WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER)
	+ pbih->biClrUsed * sizeof(RGBQUAD),
	(LPDWORD)&dwTmp, (NULL)))*/
	//	errhandler("WriteFile", hwnd);
	judge = WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER)
		+ pbih->biClrUsed * sizeof(RGBQUAD),
		(LPDWORD)&dwTmp, (NULL)); if (judge == false) MessageBox(hwnd, "6", "6", NULL);
	// Copy the array of color indices into the .BMP file.  
	//	dwTotal = cb = pbih->biSizeImage;
	cb = pbih->biSizeImage;
	dwTotal = cb;
	hp = lpBits;
	/*	if (!WriteFile(hf, (LPSTR)hp, (int)cb, (LPDWORD)&dwTmp, NULL))
	errhandler("WriteFile", hwnd);*/
	judge = WriteFile(hf, (LPSTR)hp, (int)cb, (LPDWORD)&dwTmp, NULL); if (judge == false) MessageBox(hwnd, "7", "7", NULL);
	// Close the .BMP file.  
	//	if (!CloseHandle(hf))
	//	errhandler("CloseHandle", hwnd);
	CloseHandle(hf);
	// Free memory.  
	GlobalFree((HGLOBAL)lpBits);
}

PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)
{
	BITMAP bmp;
	PBITMAPINFO pbmi;
	WORD    cClrBits;

	// Retrieve the bitmap color format, width, and height.  
	//	if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp))
	//	errhandler("GetObject", hwnd);
	bool judge = GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp); if (judge == false) MessageBox(hwnd, "1", "1", NULL);
	// Convert the color format to a count of bits.  
	cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
	if (cClrBits == 1)
		cClrBits = 1;
	else if (cClrBits <= 4)
		cClrBits = 4;
	else if (cClrBits <= 8)
		cClrBits = 8;
	else if (cClrBits <= 16)
		cClrBits = 16;
	else if (cClrBits <= 24)
		cClrBits = 24;
	else cClrBits = 32;

	// Allocate memory for the BITMAPINFO structure. (This structure  
	// contains a BITMAPINFOHEADER structure and an array of RGBQUAD  
	// data structures.)  

	if (cClrBits < 24)
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
			sizeof(BITMAPINFOHEADER) +
			sizeof(RGBQUAD) * (1 << cClrBits));

	// There is no RGBQUAD array for these formats: 24-bit-per-pixel or 32-bit-per-pixel 

	else
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
			sizeof(BITMAPINFOHEADER));

	// Initialize the fields in the BITMAPINFO structure.  

	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth = bmp.bmWidth;
	pbmi->bmiHeader.biHeight = bmp.bmHeight;
	pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
	pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
	if (cClrBits < 24)
		pbmi->bmiHeader.biClrUsed = (1 << cClrBits);

	// If the bitmap is not compressed, set the BI_RGB flag.  
	pbmi->bmiHeader.biCompression = BI_RGB;

	// Compute the number of bytes in the array of color  
	// indices and store the result in biSizeImage.  
	// The width must be DWORD aligned unless the bitmap is RLE 
	// compressed. 
	pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits + 31) & ~31) / 8
		* pbmi->bmiHeader.biHeight;
	// Set biClrImportant to 0, indicating that all of the  
	// device colors are important.  
	pbmi->bmiHeader.biClrImportant = 0;
	return pbmi;
}


/*
	常用方法封装
*/
string getWindowText(HWND hwnd) {
	char str[80];
	GetWindowText(hwnd, str, 80);
	string a = str;
	return a;
}

void setWindowText(HWND hwnd, string str) {
	SetWindowText(hwnd, str.c_str());
}

int DrawText_GUI(HDC hdc, string text, int x, int y, int color = BLACK_COLOR)
{
	SetTextColor(hdc, color);
	SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, x, y, text.c_str(), text.length());
	return 1;
}

string ToString(int IntValue) {
	char temp[64];
	sprintf_s(temp, "%d", IntValue);
	string str(temp);
	return str;
}

string DoubleToString(double IntValue) {
	char temp[64];
	sprintf_s(temp, "%.2f", IntValue);
	string str(temp);
	return str;
}


/*
 *	普通的绘制直线方法
 */
void drawALine(HDC hdc, POINT origin, POINT destination, int color = BLACK_COLOR, int width = 2) {
	HPEN pen = CreatePen(PS_SOLID, width, color);
	SelectObject(hdc, pen);
	MoveToEx(hdc, origin.x, origin.y, NULL);
	LineTo(hdc, destination.x, destination.y);
	DeleteObject(pen);
}

/*
 *	绘制长方形
 */
void drawARectangle(HDC hdc, POINT origin, POINT destination, int color = BLACK_COLOR)
{
	// this function uses Win32 API to draw a filled rectangle   
	HPEN hpen = CreatePen(PS_DASH, 2, color);
	SelectObject(hdc, hpen);
	Rectangle(hdc, origin.x, origin.y, destination.x, destination.y);
	DeleteObject(hpen);
}

/*
*	消除锯齿绘制直线方法
*/
void RVS_drawLine_width(HDC hDC, int x1, int y1, int x2, int y2, COLORREF color)
{
	// Calculate line params
	int dx = (x2 - x1);
	int dy = (y2 - y1);
	COLORREF bgColor;
	int temp;
	float k;

	// X-dominant line
	if (abs(dx) > abs(dy))
	{
		// Ex-change line end points
		if (dx < 0)
		{
			temp = x1;
			x1 = x2;
			x2 = temp;

			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		k = (float)dy / (float)dx;

		int xs;
		float yt = (float)y1;
		float distance;
		UCHAR tred, tgreen, tblue;


		for (xs = x1; xs <= x2; xs++)
		{
			distance = (float)(yt - (int)(yt));

			bgColor = ::GetPixel(hDC, xs, (int)yt - 1);
			tred = (UCHAR)(distance*GetRValue(bgColor)) + (UCHAR)((1.0f - distance)*GetRValue(color));
			tgreen = (UCHAR)(distance*GetGValue(bgColor)) + (UCHAR)((1.0f - distance)*GetGValue(color));
			tblue = (UCHAR)(distance*GetBValue(bgColor)) + (UCHAR)((1.0f - distance)*GetBValue(color));
			::SetPixel(hDC, xs, (int)yt - 1, RGB(tred, tgreen, tblue));

			//::SetPixel(hDC, xs, (int)yt-1, color);
			::SetPixel(hDC, xs, (int)yt, color);


			bgColor = ::GetPixel(hDC, xs, (int)yt + 1);
			tred = (UCHAR)((1.0f - distance)*GetRValue(bgColor)) + (UCHAR)(distance*GetRValue(color));
			tgreen = (UCHAR)((1.0f - distance)*GetGValue(bgColor)) + (UCHAR)(distance*GetGValue(color));
			tblue = (UCHAR)((1.0f - distance)*GetBValue(bgColor)) + (UCHAR)(distance*GetBValue(color));
			::SetPixel(hDC, xs, (int)yt + 1, RGB(tred, tgreen, tblue));

			yt += k;
		}

	}
	// Y-dominant line
	else
	{
		// Ex-change line end points
		if (dy < 0)
		{
			temp = x1;
			x1 = x2;
			x2 = temp;

			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		k = (float)dx / (float)dy;

		int ys;
		float xt = (float)x1;
		float distance;
		UCHAR tred, tgreen, tblue;
		for (ys = y1; ys <= y2; ys++)
		{
			distance = (float)(xt - (int)(xt));

			bgColor = ::GetPixel(hDC, (int)xt - 1, ys);
			tred = (UCHAR)(distance*GetRValue(bgColor)) + (UCHAR)((1.0f - distance)*GetRValue(color));
			tgreen = (UCHAR)(distance*GetGValue(bgColor)) + (UCHAR)((1.0f - distance)*GetGValue(color));
			tblue = (UCHAR)(distance*GetBValue(bgColor)) + (UCHAR)((1.0f - distance)*GetBValue(color));
			::SetPixel(hDC, (int)xt - 1, ys, RGB(tred, tgreen, tblue));

			//::SetPixel(hDC, (int)xt-1, ys, color);
			::SetPixel(hDC, (int)xt, ys, color);

			bgColor = ::GetPixel(hDC, (int)xt + 1, ys);
			tred = (UCHAR)((1.0f - distance)*GetRValue(bgColor)) + (UCHAR)(distance*GetRValue(color));
			tgreen = (UCHAR)((1.0f - distance)*GetGValue(bgColor)) + (UCHAR)(distance*GetGValue(color));
			tblue = (UCHAR)((1.0f - distance)*GetBValue(bgColor)) + (UCHAR)(distance*GetBValue(color));
			::SetPixel(hDC, (int)xt + 1, ys, RGB(tred, tgreen, tblue));

			xt += k;
		}
	}

}
void drawASmoothLine(HDC hdc, POINT origin, POINT destination, int color = BLACK_COLOR)
{
	HPEN pen = CreatePen(PS_SOLID, 2 , color);
	SelectObject(hdc, pen);
	RVS_drawLine_width(hdc, origin.x, origin.y, destination.x, destination.y, color);
	DeleteObject(pen);
}


/*
*	绘制网格方法
*/
void createGrid(HDC hdc, POINT zeroPoint, int nowInterval)
{
	LONG xValue = zeroPoint.x;
	LONG yValue = zeroPoint.y;

	//原点之左
	while (xValue > WINDOW_INITIAL_X - 10)
	{
		drawALine(hdc, { xValue, 0 }, { xValue,  WINDOW_INITIAL_Y + WINDOW_HEIGHT }, GRAY_COLOR, 1);
		//	OutputDebugString(ToString(xValue).c_str());
		xValue -= nowInterval;
	}
	xValue = zeroPoint.x;
	while ( xValue <= (WINDOW_INITIAL_X + WINDOW_WIDTH))
	{
		/*if (xValue < 0)
		{
			xValue += nowInterval;

			break;
		}*/
		drawALine(hdc, { xValue, 0 }, { xValue, WINDOW_HEIGHT + WINDOW_INITIAL_Y }, GRAY_COLOR, 1);
		xValue += nowInterval;
	}
	xValue = zeroPoint.x;
	//原点之上
	while (yValue > WINDOW_INITIAL_Y - 10)
	{
		drawALine(hdc, { 0, yValue }, { WINDOW_WIDTH + WINDOW_INITIAL_X, yValue }, GRAY_COLOR, 1);
		yValue -= nowInterval;
	}
	yValue = zeroPoint.y;
	while (yValue < (WINDOW_INITIAL_Y + WINDOW_HEIGHT))
	{
		drawALine(hdc, { 0, yValue }, { WINDOW_WIDTH + WINDOW_INITIAL_X, yValue }, GRAY_COLOR, 1);
		yValue += nowInterval;
	}
	yValue = zeroPoint.y;
	
	if (xValue > WINDOW_INITIAL_X)
	{
		drawALine(hdc, { xValue, 0 }, { xValue, 2 * WINDOW_HEIGHT + WINDOW_INITIAL_Y }, BLACK_COLOR, 3);
	}
	if (yValue > WINDOW_INITIAL_Y)
	{
		drawALine(hdc, { 0  , yValue }, {2 * WINDOW_WIDTH + WINDOW_INITIAL_X, yValue }, BLACK_COLOR, 3);
	}

}
/*
	绘制Tick 和number方法
*/
void createTickAndNumber(HDC hdc, POINT zeroPoint, int nowInterval)
{
	LONG xValue = zeroPoint.x;
	LONG yValue = zeroPoint.y;
	int value = 0;
	//原点之左
	while (xValue > WINDOW_INITIAL_X - 10)
	{
		drawALine(hdc, { xValue,yValue - nowInterval / 4 }, { xValue,yValue }, BLACK_COLOR, 3);
		if (value % 10 == 0)
			DrawText_GUI(hdc, ToString(value), xValue - nowInterval / 3, yValue + nowInterval / 10, BLACK_COLOR);
		xValue -= nowInterval;
		value -= 1;
	}
	xValue = zeroPoint.x;
	value = 0;
	while (xValue <= (WINDOW_INITIAL_X + WINDOW_WIDTH))
	{
		drawALine(hdc, { xValue,yValue - nowInterval / 4 }, { xValue,yValue }, BLACK_COLOR, 3);
		if (value % 10 == 0)
			DrawText_GUI(hdc, ToString(value), xValue - nowInterval / 3, yValue + nowInterval / 10, BLACK_COLOR);
		xValue += nowInterval;
		value += 1;
	}
	xValue = zeroPoint.x;
	value = 0;
	//原点之上
	while (yValue > WINDOW_INITIAL_Y - 10)
	{
		drawALine(hdc, { xValue, yValue }, { xValue + nowInterval / 4, yValue }, BLACK_COLOR, 3);
		if (value != 0 && value % 10 == 0)
			DrawText_GUI(hdc, ToString(value), xValue + nowInterval / 3, yValue - nowInterval / 5, BLACK_COLOR);
		yValue -= nowInterval;
		value += 1;
	}
	yValue = zeroPoint.y;
	value = 0;
	while (yValue < (WINDOW_INITIAL_Y + WINDOW_HEIGHT))
	{
		drawALine(hdc, { xValue, yValue }, { xValue + nowInterval / 4, yValue }, BLACK_COLOR, 3);
		if (value != 0 && value % 10 == 0)
			DrawText_GUI(hdc, ToString(value), xValue + nowInterval / 3, yValue - nowInterval / 5, BLACK_COLOR);
		yValue += nowInterval;
		value -= 1;
	}
	yValue = zeroPoint.y;

}

/*
	解析字符串为函数表达式
*/
double calculate1(double nowX, string cal, int length)
{
	char sign1[200];
	double finalNum[200];

	int zzAll;//后缀表达式栈指针 
	int zzChar;//运算符栈指针 
	int zzFinal;//计算后缀表达式的数栈 
	int testNum;// 检测当前数字是否多位 
	int testPoint;
	int countPoint;
	int testCorrect;
	char nowSign;

	zzAll = 0;
	zzChar = 0;
	zzFinal = 0;
	testNum = 0;
	testPoint = 0;
	countPoint = 0;
	testCorrect = 1;
	for (int i = 0; i<length - 1; i++)

	{

		nowSign = cal[i];


		if (nowSign != '\n')
		{
			if (nowSign - '0' >= 0 && nowSign - '0' <= 9)
			{
				if (testNum == 0 && testPoint == 0)
				{
					all[zzAll].find = 1;
					all[zzAll].num = nowSign - '0';
					zzAll++;
					testNum = 1;
				}
				else
				{
					if (testNum)all[zzAll - 1].num = all[zzAll - 1].num * 10 + nowSign - '0';
					else
					{
						if (testPoint == 1)
						{
							all[zzAll - 1].num = all[zzAll - 1].num + pow(10, -countPoint)*(nowSign - '0');
							countPoint++;
						}
						else
						{
							testCorrect = 0;
							return -999999997;
						}
					}
				}

			}
			else if (nowSign == 'x')
			{
				all[zzAll].find = 1;
				all[zzAll].num = nowX;
				zzAll++;
				testNum = 0;
				testPoint = 0;
				countPoint = 0;
			}
			else if (nowSign == '.')
			{
				testPoint++;
				countPoint = 1;
				testNum = 0;
			}


			else
			{

				testNum = 0;
				testPoint = 0;
				countPoint = 0;
				if (zzChar == 0 && (nowSign == '+' || nowSign == '-' || nowSign == '*' || nowSign == '/' || nowSign == '^' || nowSign == 'l' || nowSign == 'a' || nowSign == 's' || nowSign == 'c'))
				{


					sign1[0] = nowSign;
					zzChar++;
					if (nowSign == 'l' || nowSign == 'a' || nowSign == 's' || nowSign == 'c')i = i + 2;


				}
				else
				{
					if (nowSign == '^')
					{
						/*while(sign1[zzChar-1]=='^')
						{
						all[zzAll].find=0;
						all[zzAll].sign=sign1[zzChar-1];
						zzChar--;
						zzAll++;
						}*/
						sign1[zzChar] = nowSign;
						zzChar++;
					}
					else if (nowSign == '*' || nowSign == '/')
					{
						while (sign1[zzChar - 1] == '^' || sign1[zzChar - 1] == '*' || sign1[zzChar - 1] == '/' || sign1[zzChar - 1] == 'l' || sign1[zzChar - 1] == 'a' || sign1[zzChar - 1] == 's' || sign1[zzChar - 1] == 'c')//运算符高者出栈 
						{
							all[zzAll].find = 0;
							all[zzAll].sign = sign1[zzChar - 1];
							zzChar--;
							zzAll++;
						}
						sign1[zzChar] = nowSign;
						zzChar++;
					}
					else if (nowSign == '+' || nowSign == '-')
					{

						while (sign1[zzChar - 1] == '^' || sign1[zzChar - 1] == '*' || sign1[zzChar - 1] == '/' || sign1[zzChar - 1] == '+' || sign1[zzChar - 1] == '-' || sign1[zzChar - 1] == 'l' || sign1[zzChar - 1] == 'a' || sign1[zzChar - 1] == 's' || sign1[zzChar - 1] == 'c')
						{
							all[zzAll].find = 0;
							all[zzAll].sign = sign1[zzChar - 1];
							zzChar--;
							zzAll++;
						}
						sign1[zzChar] = nowSign;
						zzChar++;

					}
					else if (nowSign == 'l')
					{

						i = i + 2;
						/*while(sign1[zzChar-1]=='^')
						{
						all[zzAll].find=0;
						all[zzAll].sign=sign1[zzChar-1];
						zzChar--;
						zzAll++;
						}*/
						sign1[zzChar] = nowSign;
						zzChar++;
					}
					else if (nowSign == 'a')
					{
						i = i + 2;
						/*while(sign1[zzChar-1]=='^')
						{
						all[zzAll].find=0;
						all[zzAll].sign=sign1[zzChar-1];
						zzChar--;
						zzAll++;
						}*/
						sign1[zzChar] = nowSign;
						zzChar++;
					}
					else if (nowSign == 's')
					{
						i = i + 2;

						/*while(sign1[zzChar-1]=='^')
						{
						all[zzAll].find=0;
						all[zzAll].sign=sign1[zzChar-1];
						zzChar--;
						zzAll++;
						}*/
						sign1[zzChar] = nowSign;
						zzChar++;
					}
					else if (nowSign == 'c')
					{
						i = i + 2;

						/*while(sign1[zzChar-1]=='^')
						{
						all[zzAll].find=0;
						all[zzAll].sign=sign1[zzChar-1];
						zzChar--;
						zzAll++;
						}*/
						sign1[zzChar] = nowSign;
						zzChar++;
					}

					else if (nowSign == '(')
					{
						sign1[zzChar] = nowSign;
						zzChar++;

					}
					else if (nowSign == ')')
					{
						while (sign1[zzChar - 1] != '(')
						{
							all[zzAll].find = 0;
							all[zzAll].sign = sign1[zzChar - 1];
							zzChar--;
							zzAll++;
							if (zzChar <= 0)
							{
								testCorrect = 0;
								return -999999999;
							}
						}
						zzChar--;
					}
					else
					{
						testCorrect = 0;
						return -999999998;

					}




				}



			}

		}

	}
	//算式读完多余运算符出栈 
	while (zzChar&&testCorrect)
	{
		if (sign1[zzChar - 1] != '(')
		{
			all[zzAll].find = 0;
			all[zzAll].sign = sign1[zzChar - 1];
			zzChar--;
			zzAll++;
		}
		else
		{
			testCorrect = 0;
			return -999999999;
		}
	}
	//后缀表达式计算 
	for (int i = 0; i<zzAll; i++)
	{
		if (all[i].find)
		{
			finalNum[zzFinal] = all[i].num;
			zzFinal++;
		}
		else
		{
			if (all[i].sign == '^')
			{
				finalNum[zzFinal - 2] = pow(finalNum[zzFinal - 2], finalNum[zzFinal - 1]);
				zzFinal = zzFinal - 1;
			}
			if (all[i].sign == '*')
			{
				finalNum[zzFinal - 2] = finalNum[zzFinal - 2] * finalNum[zzFinal - 1];
				zzFinal = zzFinal - 1;
			}
			if (all[i].sign == '/')
			{
				finalNum[zzFinal - 2] = finalNum[zzFinal - 2] / finalNum[zzFinal - 1];
				zzFinal = zzFinal - 1;
			}
			if (all[i].sign == '+')
			{
				finalNum[zzFinal - 2] = finalNum[zzFinal - 2] + finalNum[zzFinal - 1];
				zzFinal = zzFinal - 1;
			}
			if (all[i].sign == '-')
			{
				if (zzFinal >= 2)
				{
					finalNum[zzFinal - 2] = finalNum[zzFinal - 2] - finalNum[zzFinal - 1];
					zzFinal = zzFinal - 1;
				}
				else finalNum[zzFinal - 1] = -finalNum[zzFinal - 1];

			}
			if (all[i].sign == 'l')finalNum[zzFinal - 1] = log2(finalNum[zzFinal - 1]);
			if (all[i].sign == 'a')finalNum[zzFinal - 1] = abs(finalNum[zzFinal - 1]);
			if (all[i].sign == 's')finalNum[zzFinal - 1] = sin(finalNum[zzFinal - 1]);
			if (all[i].sign == 'c')finalNum[zzFinal - 1] = cos(finalNum[zzFinal - 1]);
		}
	}
	return finalNum[0];

}