#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "weatherdata.h"
#include "weathertool.h"

#include <QMainWindow>
#include <QContextMenuEvent>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QCursor>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QString>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QPainter>

#define DRAW_HIGH_CURVE 0	//画高温曲线
#define DRAW_LOW_CURVE 1	//画低温曲线

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void onReplied(QNetworkReply* reply);

private:
	Ui::MainWindow *ui;

	QMenu *mExitMenu;       //右键菜单
    QPoint mOffset;         //鼠标到窗口的偏移量
    QNetworkAccessManager *mNetworkAccessManager;
    Today todayWeather;     //今天的天气信息
    Day day[6];             //六天的天气信息
    QList<QLabel *> mDateList;  //日期标签列表
    QList<QLabel *> mWeekList;  //星期标签列表
    QList<QLabel *> mQualityList;    //空气质量标签列表
    QList<QLabel *> mTypeList;  //天气类型标签列表（晴阴）
    QList<QLabel *> mTypeIconList;  //天气类型图标标签（晴阴）
    QList<QLabel *> mFxList;    //风向
    QList<QLabel *> mFlList;    //风力
    QMap<QString, QString> mTypeMap;    //天气类型和类型图片的关联Map

	void parseJson(const QByteArray& replyArray);  //解析Json字符串
	void updateUI();    //将解析出的数据，更新到界面显示
    void getWeatherInfo(const QString& cityCode);  //获取某个城市的天气信息
    int parseWenduString(const QString& wenduStr);
};

#endif // MAINWINDOW_H
