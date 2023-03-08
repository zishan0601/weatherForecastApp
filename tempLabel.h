#pragma once
#include <QLabel>
#include <QPainter>
#include <QDebug>

#include "mainwindow.h"

#define DRAW_HIGH_CURVE 0	//画高温曲线
#define DRAW_LOW_CURVE 1	//画低温曲线
#define TEMP_OFFSET 2	//温度曲线，每升高/降低1度变化的像素
#define PAINT_RADIUS 3      //曲线描点的大小

class TempLabel :
	public QLabel
{
public:
	TempLabel(QWidget *);
	//这里将画好闻曲线和低温曲线放在一个函数中了，drawHighOrLow是判断画高温还是低温的参数标志
	void tempPosition(const int drawHighOrLow, const Day dayArr[], const int size);

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	int tempHigh[6];	//高温温度
	int tempLow[6];		//低温温度
	int posX[6];
	int posYHigh[6];
	int posYLow[6];
	int Flag_drawHighOrLow = -1;	//高低温曲线的flag
	int daySize;	//天气预报的天数
};

