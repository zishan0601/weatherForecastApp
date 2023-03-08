QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += ./tempLabel.h \
    ./weatherdata.h \
    ./weathertool.h \
    ./mainwindow.h

SOURCES += ./TempLabel.cpp \
    ./main.cpp \
    ./mainwindow.cpp \
    ./weathertool.cpp

FORMS += ./mainwindow.ui

RESOURCES += main.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


RC_ICONS = tubiao.ico
