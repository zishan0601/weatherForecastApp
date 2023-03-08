#include "tempLabel.h"

TempLabel::TempLabel(QWidget*)
{
}

void TempLabel::tempPosition(const int drawHighOrLow, const Day dayArr[], const int size)
{
	Flag_drawHighOrLow = drawHighOrLow;
	TempLabel::daySize = size;
	//1.获取高低温 平均温度
	int highTempSum = 0;
	int lowTempSum = 0;
	for (int i = 0; i < daySize; ++i)
	{
		tempHigh[i] = dayArr[i].high;
		tempLow[i] = dayArr[i].low;
		highTempSum = highTempSum + dayArr[i].high;
		lowTempSum = lowTempSum + dayArr[i].low;
	}
	int highTempAver = highTempSum / size;
	int lowTempAver = lowTempSum / size;

	//2.获取每个点的x坐标
	//3.获取每个点的y坐标，高温度向上偏移
	// 温度曲线标签的y轴中线位置
	int lblMidY = this->height() / 2;

	for (int i = 0; i < daySize; ++i)
	{
		//每个点的x轴位置
		posX[i] = (1 + 2 * i) * (this->width() / 12);
		//高低温y轴的位置
		posYHigh[i] = lblMidY - (dayArr[i].high - highTempAver) * TEMP_OFFSET;
		posYLow[i] = lblMidY - (dayArr[i].low - lowTempAver) * TEMP_OFFSET;
	}
}

void TempLabel::paintEvent(QPaintEvent* event)
{
	//4.在高低温模块上画曲线
	QPainter painter;
	QPen pen;
	pen.setWidth(1);

	QFont font;
	font.setPixelSize(14);

	painter.begin(this);
	painter.setRenderHints(QPainter::Antialiasing); //抗锯齿
	//根据高温和低温曲线来画图
	switch (TempLabel::Flag_drawHighOrLow)
	{
	case DRAW_HIGH_CURVE:
		{
			pen.setColor(QColor(255, 170, 0));
			painter.setBrush(QBrush(QColor(255, 170, 0)));
			painter.setPen(pen);
			painter.setFont(font);

			for (int i = 0; i < daySize; i++)
			{
				painter.drawEllipse(QPoint(posX[i], posYHigh[i]), PAINT_RADIUS, PAINT_RADIUS);
				//写文字
                QString temp = QString::number(tempHigh[i]) + "°";
				painter.drawText(QPoint(posX[i] - 3, posYHigh[i] - 7), temp);
				if (i == 0)
				{
					pen.setStyle(Qt::DashLine);
					painter.setPen(pen);
					painter.drawLine(posX[i], posYHigh[i], posX[i + 1], posYHigh[i + 1]);
				}
				else if ((i > 0) & (i < (daySize - 1)))
				{
					pen.setStyle(Qt::SolidLine);
					painter.setPen(pen);
					painter.drawLine(posX[i], posYHigh[i], posX[i + 1], posYHigh[i + 1]);
				}
			}
			painter.end();
            qDebug() << "高温 画图完成了";
			break;
		}
	case DRAW_LOW_CURVE:
		{
			pen.setColor(QColor(0, 255, 255));
			painter.setBrush(QBrush(QColor(0, 255, 255)));
			painter.setPen(pen);
			painter.setFont(font);

			for (int i = 0; i < daySize; i++)
			{
				painter.drawEllipse(QPoint(posX[i], posYLow[i]), PAINT_RADIUS, PAINT_RADIUS);
				//写文字
                QString temp = QString::number(tempLow[i]) + "°";
				painter.drawText(QPoint(posX[i] - 3, posYLow[i] - 7), temp);
				if (i == 0)
				{
					pen.setStyle(Qt::DashLine);
					painter.setPen(pen);
					painter.drawLine(posX[i], posYLow[i], posX[i + 1], posYLow[i + 1]);
				}
				else if ((i > 0) & (i < (daySize - 1)))
				{
					pen.setStyle(Qt::SolidLine);
					painter.setPen(pen);
					painter.drawLine(posX[i], posYLow[i], posX[i + 1], posYLow[i + 1]);
				}
			}
			painter.end();
            qDebug() << "低温 画图完成了";
			break;
		}
	case -1:
		break; //空调用，没有高低温曲线输入，直接结束
	default:
		{
			qDebug() << "the draw High Or Low curve instruction err! Flag_drawHighOrLow = " << Flag_drawHighOrLow;
			break;
		}
	}
}
