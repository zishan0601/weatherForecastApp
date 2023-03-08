#ifndef WEATHERDATA_H
#define WEATHERDATA_H

#include <QString>

class Today
{
public:
    //今天的温度数据
    Today()
    {
        date = "2022-10-20";
        city = "广州";

        ganmao = "感冒指数";

        wendu = 10;
        shidu = "10%";
        pm25 = 2;
        quality = "无数据";

        type = "多云";

        fl = "2级";
        fx = "南风";

        high = 30;
        low = 18;
    }

    QString date;   //日期
    QString city;   //城市

    QString ganmao; //感冒指数

    int wendu;      //温度
    QString shidu;  //湿度
    int pm25;       //PM2.5
    QString quality;//空气质量，文字描述

    QString type;   //天气类型，多云/下雨

    QString fx;     //风向
    QString fl;     //风力

    int high;       //高温
    int low;        //低温
};

//除今天外的其他天气温度
class Day
{
public:
    Day()
    {
        date = "2022-10-20";
        week = "周五";

        type = "多云";

        high = 0;
        low = 0;

        fx = "南风";
        fl = "2级";

        aqi = 0;
    }

    QString date;   //日期，某月的某天
    QString week;   //星期

    QString type;   //天气类型
    QString ymd;    //日期xxxx-xx-xx

    int high;       //高温
    int low;        //低温

    QString fx;     //风向
    QString fl;     //风力

    int aqi;        //空气指数
};

#endif // WEATHERDATA_H
