#ifndef WEATHERTOOL_H
#define WEATHERTOOL_H

#include <QFile>
#include <QMap>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>

class weatherTool
{
public:
    weatherTool();
    static QString getWeatherCode(const QString& cityName);

private:
    static void initCityMap();  //将Json文件中的内容转化成CityMap
    static QMap<QString, QString> mCityMap; //城市名和城市编号的Map
};

#endif // WEATHERTOOL_H
