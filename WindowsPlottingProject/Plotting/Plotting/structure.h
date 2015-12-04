#pragma once

#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

//	每一条线
struct MathExpression
{
	string expressionStr;
	int lineColor;
	int lineWidth;
	int splitCount;
	MathExpression() {

	}
	MathExpression(int color) {
		lineColor = color;
	}
};

//	图像集合
struct Graph
{
	vector<MathExpression> graphSet;
	Graph() {
		graphSet = vector<MathExpression>();
		for (int i = 0; i < SUPPORTED_FUNCTION_SIZE; i++)
		{
			MathExpression temp = MathExpression();
			temp.expressionStr = "";
			graphSet.push_back(temp);
		}
	}
};

//	坐标转换用的结构体
struct DoulePoint {
	double x;
	double y;
	DoulePoint(double _x, double _y) {
		x = _x;
		y = _y;
	}
};

//node
struct node
{
	int find;
	double num;
	char sign;
};