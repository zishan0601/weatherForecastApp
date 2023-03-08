#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //窗口无边框
    //this->setWindowFlag(Qt::FramelessWindowHint);
    this->setWindowTitle("天气预报小程序");
    this->setFixedSize(800,450);

	//为高低温模块，安装事件过滤器
	//ui->lblHighCurve->installEventFilter(this);
	//ui->lblLowCurve->installEventFilter(this);

    //构建右键的退出菜单，和退出动作
    mExitMenu = new QMenu(this);
    QAction *mExitAction = new QAction(this);
    mExitAction->setText("退出");
    mExitAction->setIcon(QIcon(":/res/close.png"));
    mExitMenu->addAction(mExitAction);

    //信号槽，点击右键的退出按钮后，关闭应用
    connect(mExitAction, &QAction::triggered, this, [=](){
        qApp->exit();
    });

    //Http接收数据
    mNetworkAccessManager = new QNetworkAccessManager(this);
    //从网络获取当前城市的天气信息
    getWeatherInfo("北京");
    /*
     * http获取成功后，触发finished信号
     * finished信号自带一个QNetworkReply *reply，所以槽函数可以收到这个参数
    */
    connect(mNetworkAccessManager, &QNetworkAccessManager::finished, this, &MainWindow::onReplied);

    //将UI控件放到数组里边,方便使用循环进行处理
    mDateList << ui->lblDate0 << ui->lblDate1 << ui->lblDate2 << ui->lblDate3 << ui->lblDate4 << ui->lblDate5;
    mWeekList << ui->lblWeek0 << ui->lblWeek1 << ui->lblWeek2 << ui->lblWeek3 << ui->lblWeek4 << ui->lblWeek5;
    mQualityList << ui->lblQuality0 << ui->lblQuality1 << ui->lblQuality2 << ui->lblQuality3 << ui->lblQuality4 << ui->lblQuality5;
    mTypeList << ui->lblType0 << ui->lblType1 << ui->lblType2 << ui->lblType3 << ui->lblType4 << ui->lblType5;
    mTypeIconList << ui->lblTypeIcon0 << ui->lblTypeIcon1 << ui->lblTypeIcon2 << ui->lblTypeIcon3 << ui->lblTypeIcon4 << ui->lblTypeIcon5;
    mFxList << ui->lblFx0 << ui->lblFx1 << ui->lblFx2 << ui->lblFx3 << ui->lblFx4 << ui->lblFx5;
    mFlList << ui->lblFl0 << ui->lblFl1 << ui->lblFl2 << ui->lblFl3 << ui->lblFl4 << ui->lblFl5;

    //将天气类型和图片的路径关联起来，便于切换图片
    mTypeMap.insert("暴雪",":/res/type/BaoXue.png");
    mTypeMap.insert("暴雨",":/res/type/BaoYu. png");
    mTypeMap.insert("暴雨到大暴雨",":/res/type/BaoYuDaoDaBaoYu.png");
    mTypeMap.insert("大暴雨",":/res/type/DaBaoYu.png");
    mTypeMap.insert("大暴雨到特大暴雨",":/res/type/DaBaoYuDaoTeDaBaoYu.png");
    mTypeMap.insert("大到暴雪",":/res/type/DaDaoBaoXue.png");
    mTypeMap.insert("大雪",":/res/type/DaXue.png");
    mTypeMap.insert("大雨",":/res/type/DaYu.png");
    mTypeMap.insert("冻雨",":/res/type/DongYu.png");
    mTypeMap.insert("多云",":/res/type/DuoYun.png");
    mTypeMap.insert("浮沉",":/res/type/FuChen.png");
    mTypeMap.insert("雷阵雨",":/res/type/LeiZhenYu.png");
    mTypeMap.insert("雷阵雨伴有冰雹",":/res/type/LeiZhenYuBanYouBingBao.png");
    mTypeMap.insert("霾",":/res/type/Mai.png");
    mTypeMap.insert("强沙尘暴",":/res/type/QiangShaChenBao.png");
    mTypeMap.insert("晴",":/res/type/Qing.png");
    mTypeMap.insert("沙尘暴",":/res/type/ShaChenBao.png");
    mTypeMap.insert("特大暴雨",":/res/type/TeDaBaoYu.png");
    mTypeMap.insert("undefined",":/res/type/undefined.png");
    mTypeMap.insert("雾",":/res/type/Wu.png");
    mTypeMap.insert("小到中雪",":/res/type/XiaoDaoZhongXue.png");
    mTypeMap.insert("小到中雨",":/res/type/XiaoDaoZhongYu.png");
    mTypeMap.insert("小雪",":/res/type/XiaoXue.png");
    mTypeMap.insert("小雨",":/res/type/XiaoYu.png");
    mTypeMap.insert("雪",":/res/type/Xue.png");
    mTypeMap.insert("扬沙",":/res/type/YangSha.png");
    mTypeMap.insert("阴",":/res/type/Yin.png");
    mTypeMap.insert("雨",":/res/type/Yu.png");
    mTypeMap.insert("雨夹雪",":/res/type/YuJiaXue.png");
    mTypeMap.insert("阵雪",":/res/type/ZhenXue.png");
    mTypeMap.insert("阵雨",":/res/type/ZhenYu.png");
    mTypeMap.insert("中到大雪",":/res/type/ZhongDaoDaXue.png");
    mTypeMap.insert("中到大雨",":/res/type/ZhongDaoDaYu.png");
    mTypeMap.insert("中雪",":/res/type/ZhongXue.png");
    mTypeMap.insert("中雨",":/res/type/ZhongYu.png");

    //设置搜索天气的槽函数
    connect(ui->btnSearch, &QPushButton::clicked, this, [=](){
        getWeatherInfo(ui->leCity->text());
		//这里无需再次创建信号槽，getWeatherInfo会触发QNetworkAccessManager::finished信号，直接关联上面创建好的槽函数
        //connect(mNetworkAccessManager, &QNetworkAccessManager::finished, this, &MainWindow::onReplied, Qt::UniqueConnection);
    });
}

/*
 * 重写父类的虚函数,父类中默认的实现是忽略右健菜单事件
 * 重写之后，可以处理右键菜单事件
 */
void MainWindow::contextMenuEvent(QContextMenuEvent *event){
    //弹出右键菜单
    mExitMenu->exec(QCursor::pos());
    //事件已经处理,不需要向上传，其实默认是不向上传，可以不写
    event->accept();
}

/*
 * 鼠标按下事件，记录按下的位置信息和到窗口左上角的偏移量
 */
void MainWindow::mousePressEvent(QMouseEvent *event){
    //globalPos()是到整个屏幕左上角的坐标，pos()是当前窗口左上角到屏幕左上角的坐标位置
    mOffset = event->globalPos() - this->pos();
}


/*
 * 鼠标移动事件，记录移动后的位置
 */
void MainWindow::mouseMoveEvent(QMouseEvent *event){
    this->move(event->globalPos() - mOffset);
}

void MainWindow::getWeatherInfo(const QString& cityName)
{
    QString cityCode = weatherTool::getWeatherCode(cityName);
    //城市代码为空，城市名称输入错误
    if(cityCode == "")
    {
        QMessageBox::warning(this, "天气", "城市名称输入错误", QMessageBox::Ok);
        return;
    }

    QUrl url("http://t.weather.itboy.net/api/weather/city/" + cityCode);
    mNetworkAccessManager->get(QNetworkRequest(url));
}

void MainWindow::onReplied(QNetworkReply *reply)
{
    //获取请求到的响应码参数 正常是200
    int stateCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    //打印几项reply得到的参数
    qDebug() << "operation" << reply->operation();
    qDebug() << "stateCode" << stateCode;
    qDebug() << "url" << reply->url();
    qDebug() << "rawHeader" << reply->rawHeaderList();

    if(reply->error() != QNetworkReply::NoError || stateCode != 200)
    {
        qDebug() << reply->errorString().toLocal8Bit().data();
        QMessageBox::warning(this, "天气", "请求数据失败", QMessageBox::Ok);
    }else
    {
        QByteArray replyArray = reply->readAll();
        //QByteArray::data()返回这个数byte数组的指针，这样才能正常打印
        qDebug() << replyArray.data();
        //解析json字符串
        parseJson(replyArray);
    }
}

void MainWindow::parseJson(const QByteArray& replyArray){
    //QJsonParseError存放Json读取错误的报错
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(replyArray, &err);
    if(err.error != QJsonParseError::NoError){
        qDebug() << "parse Json is error!" << "  " << err.errorString();
        return;
    }
    QJsonObject obj = doc.object();
    QJsonObject ObjCityInfo = obj.value("cityInfo").toObject();

    QJsonObject objAllWeather = obj.value("data").toObject();
    //解析昨天的天气
    Day yesterday;
    QJsonObject objYesterday = objAllWeather.value("yesterday").toObject();
    yesterday.date = objYesterday.value("date").toString();
    yesterday.week = objYesterday.value("week").toString();
    yesterday.type = objYesterday.value("type").toString();
    yesterday.ymd = objYesterday.value("ymd").toString();

    yesterday.high = parseWenduString(objYesterday.value("high").toString());
    yesterday.low = parseWenduString(objYesterday.value("low").toString());

    yesterday.fx = objYesterday.value("fx").toString();
    yesterday.fl = objYesterday.value("fl").toString();
    yesterday.aqi = objYesterday.value("aqi").toInt();
    day[0] = yesterday;

    //将最近6天的天气存储起来
    QJsonArray dayArray = objAllWeather.value("forecast").toArray();
    for (int i = 0;i < 5; i++) {
        //date是16这个值，指当天是几号
        day[i+1].date = dayArray[i].toObject().value("date").toString();
        day[i+1].week = dayArray[i].toObject().value("week").toString();
        day[i+1].type = dayArray[i].toObject().value("type").toString();
        //ymd是2022-11-16格式
        day[i+1].ymd  = dayArray[i].toObject().value("ymd").toString();

        day[i+1].high = parseWenduString(dayArray[i].toObject().value("high").toString());
        day[i+1].low  = parseWenduString(dayArray[i].toObject().value("low").toString());

        day[i+1].fx = dayArray[i].toObject().value("fx").toString();
        day[i+1].fl = dayArray[i].toObject().value("fl").toString();
        day[i+1].aqi = dayArray[i].toObject().value("aqi").toInt();
    }

    //解析当天天气信息，day[1]是今天
    todayWeather.city = ObjCityInfo.value("city").toString();
    todayWeather.date = day[1].ymd;
    todayWeather.ganmao = objAllWeather.value("ganmao").toString();
    //Json中的wendu是一个字符串，要注意转换。
    todayWeather.wendu = objAllWeather.value("wendu").toString().toInt();
    todayWeather.shidu = objAllWeather.value("shidu").toString();
    todayWeather.pm25 = objAllWeather.value("pm25").toInt();
    todayWeather.quality = objAllWeather.value("quality").toString();
    todayWeather.type = day[1].type;
    todayWeather.fx = day[1].fx;
    todayWeather.fl = day[1].fl;
    todayWeather.high = day[1].high;
    todayWeather.low = day[1].low;

    //更新到UI界面中
    updateUI();

	//更新绘图状态
	ui->lblHighCurve->tempPosition(DRAW_HIGH_CURVE, day, 6);
	ui->lblHighCurve->update();
	ui->lblLowCurve->tempPosition(DRAW_LOW_CURVE, day, 6);
	ui->lblLowCurve->update();
}

void MainWindow::updateUI()
{
    //1.更新日期和城市
    QDate tDate = QDate::fromString(todayWeather.date, "yyyy-MM-dd");
    ui->lbDate->setText(tDate.toString("yyyy/MM/dd") + " " + day[1].week);
    ui->lbCity->setText(todayWeather.city);

    //2.更新今天
    ui->lbTemp->setText(QString::number(todayWeather.wendu) + "℃");
    ui->lbType->setText(todayWeather.type);
    ui->lbTypeIcon->setPixmap(QPixmap(mTypeMap.value(todayWeather.type)));
    ui->lbLowHigh->setText(QString::number(todayWeather.low) + "~" + QString::number(todayWeather.high) + "℃");
    ui->lbGaoMao->setText("感冒指数：" + todayWeather.ganmao);
    ui->lbWindFx->setText(todayWeather.fx);
    ui->lbWindFl->setText(todayWeather.fl);
    ui->lbPm25->setText(QString::number(todayWeather.pm25));
    ui->lbShiDu->setText(todayWeather.shidu);
    ui->lblQuality->setText(todayWeather.quality);

    //3.更新其他六天
    for (int i = 0;i < 6; i++) 
	{
        //星期
        if(i != 0)
		{
            mWeekList[i]->setText(day[i].week);
        }

        //日期
        QStringList dateList = day[i].ymd.split("-");
        mDateList[i]->setText(dateList[1] + "/" + dateList[2]);

        //天气类型和图标
        mTypeList[i]->setText(day[i].type);
        mTypeIconList[i]->setPixmap(QPixmap(mTypeMap.value(day[i].type)));

        //空气质量
        if(day[i].aqi >= 0 && day[i].aqi <= 50){
            mQualityList[i]->setText("优");
            mQualityList[i]->setStyleSheet("background-color: rgb(121,184,0);");
        }
        else if (day[i].aqi >= 51 && day[i].aqi <= 100) {
            mQualityList[i]->setText("良");
            mQualityList[i]->setStyleSheet("background-color: rgb(255,187,23);");
        }
        else if (day[i].aqi >= 101 && day[i].aqi <= 150) {
            mQualityList[i]->setText("轻度");
            mQualityList[i]->setStyleSheet("background-color: rgb(255,87,97);");
        }
        else if (day[i].aqi >= 151 && day[i].aqi <= 200) {
            mQualityList[i]->setText("中度");
            mQualityList[i]->setStyleSheet("background-color: rgb(235,17,27);");
        }
        else if (day[i].aqi >= 201 && day[i].aqi <= 300) {
            mQualityList[i]->setText("重度");
            mQualityList[i]->setStyleSheet("background-color: rgb(170,0,0);");
        }
        else if (day[i].aqi >= 301) {
            mQualityList[i]->setText("严重");
            mQualityList[i]->setStyleSheet("background-color: rgb(110,0,0);");
        }

        //风向和风力
        mFxList[i]->setText(day[i].fx);
        mFlList[i]->setText(day[i].fl);
    }
}

/*
 * 将高温 18℃这种表示转成18
 */
int MainWindow::parseWenduString(const QString& wenduStr){
    //以空格分割为 高温 18℃ 这种形式为两个字符串，at(1)取后面的18℃
	QString str = wenduStr.split(" ").at(1);
    //取18℃左侧的温度值
    str = str.left(str.size() - 1);
    return str.toInt();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * 使用事件过滤器过滤，然后绘图，也可以完成绘图功能，但是程序会弹出警告，Qt不推荐使用这种方式进行绘图，如果想用课程里教的方法实现，可以参考下面的代码。
 * 在本项目中，是采用重写绘图事件来实现的，可以查看TempLabel里的paintEvent函数。
*/

/*
bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
//在高温曲线标签上，且使用画图事件时，触发本层的过滤器
if(watched == ui->lblHighCurve && event->type() == QEvent::Paint)
{
drawHighCurve(day, 6);
}
//在低温曲线标签上，且使用画图事件时，触发本层的过滤器
//if (watched == ui->lbLowHigh && event->type() == QEvent::Paint)
//{
//	drawLowCurve(day,6);
//}

//事件和过滤器执行完毕后，都要返回到其父类，让事件继续执行下去
return QMainWindow::eventFilter(watched, event);
}
*/

/*
void MainWindow::drawHighCurve(Day dayArr[], const int size)
{
//1.获取平均温度
int tempSum = 0;
for (int i = 0; i < size; ++i)
{
tempSum = tempSum + dayArr[i].high;
}
int tempAver = tempSum / size;


//2.获取每个点的x坐标
//3.获取每个点的y坐标，高温度向上偏移

// 温度曲线标签的y轴中线位置
int lblMidY = ui->lblHighCurve->height() / 2;

for (int i = 0; i < size; ++i)
{
//每个点的x轴位置
int x = 53;
int y = lblMidY + (dayArr[i].high - tempAver) * TEMP_OFFSET;
highPos[i].setX(x);
highPos[i].setY(y);
}

//4.在lblHighCurve上画高温曲线
QPainter painter(ui->lblHighCurve);
QPen pen;
pen.setColor(QColor(0, 255, 255));
pen.setWidth(1);
painter.setPen(pen);

painter.drawLine(0, 0, 10, 10);
qDebug() << "number is: " << n;
}
*/

/*
void MainWindow::drawLowCurve(Day dayArr[], const int size)
{

}
*/
