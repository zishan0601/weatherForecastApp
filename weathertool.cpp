// ReSharper disable StringLiteralTypo
#include "weathertool.h"

QMap<QString, QString> weatherTool::mCityMap;

void weatherTool::initCityMap()
{
    QString cityPath = ":/citycode.json";
    QFile cityFile(cityPath);
    if(cityFile.open(QIODevice::ReadOnly | QIODevice::Text) != true)
    {
        qDebug() << "读取文件错误";
        return;
    }
    QByteArray cityJsonString = cityFile.readAll();
    cityFile.close();

    QJsonParseError jsonErr;
    QJsonDocument cityDoc = QJsonDocument::fromJson(cityJsonString, &jsonErr);
    if(jsonErr.error != QJsonParseError::NoError)
    {
        qDebug() << "Json文件读取错误：" <<jsonErr.errorString();
        return;
    }
    QJsonArray cityJsonArray = cityDoc.array();
    if(cityJsonArray.isEmpty())
    {
        qDebug() << "Json文件中无内容";
        return;
    }

    for (QJsonArray::iterator it = cityJsonArray.begin(); it != cityJsonArray.end(); ++it)
    {
        QJsonObject mCityObj = it->toObject();
        QString cityName = mCityObj.value("city_name").toString();
        QString cityCode = mCityObj.value("city_code").toString();
        if(cityCode != "")
            mCityMap.insert(cityName, cityCode);
    }
}

QString weatherTool::getWeatherCode(const QString& cityName){
	if (mCityMap.empty())
		initCityMap();

	QString cName;
    QString cityCode = mCityMap.value(cityName);
    if(cityCode == "")
    {
        cName = QString(cityName).append("市");
		cityCode = mCityMap.value(cName);
    }
	if (cityCode == "")
	{
		cName = "";
        if (cityName.at(cityName.size() - 1) == "市"[0])
		{
			cName = cityName.left(cityName.size() - 1);
			cityCode = mCityMap.value(cName);
		}
	}
	if (cityCode == "")
	{
        cName = QString(cityName).append("县");
		cityCode = mCityMap.value(cName);
	}
	if (cityCode == "")
	{
        cName = QString(cityName).append("区");
		cityCode = mCityMap.value(cName);
	}
    return cityCode;
}

weatherTool::weatherTool()
{

}
