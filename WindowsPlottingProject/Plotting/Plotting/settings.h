#pragma once


/*窗口*/
#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 600
#define WINDOW_INITIAL_X 10
#define WINDOW_INITIAL_Y 10
#define MAXPLOTFUCTION 4


/*坐标系*/
#define POS_X 500	//坐标原点X
#define POS_Y 500	//坐标原点Y
#define POS_WIDTH 2000	//坐标系宽度
#define POS_GRID_SCALE	50	//网格初始缩放等级

#define GLOBAL_INTERVAL 30
#define GLOBAL_INTERVALValue 1

/*颜色*/
#define BLACK_COLOR RGB(0, 0, 0)	//黑色
#define RED_COLOR RGB(255, 0, 0)	//可选，红色
#define GREEN_COLOR RGB(0,255,0)	//可选，绿色
#define BLUE_COLOR RGB(0,0,255)		//可选，蓝色
#define WHITE_COLOR RGB(255,255,255)	//白色
#define GRAY_COLOR RGB(180,180,180)		//网格灰色
#define PURPLE_COLOR RGB(255,0,255)		//可选紫色
#define YELLOW_COLOR RGB(255, 180, 0)	//可选黄色
/*可选颜色数量*/
#define AVAILABLE_COLOR_COUNT 5

#define SUPPORTED_FUNCTION_SIZE 4	//最大支持函数图像数量
#define DEFAULT_SPLITCOUNT 0.01		//画线间距