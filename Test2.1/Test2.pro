QT       += core gui
QT += sql
QT += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    container.cpp \
    factury.cpp \
    main.cpp \
    mainwindow.cpp \
    sqlhelper.cpp \
    ../QRibbon/QRibbon.cpp \
    user.cpp


HEADERS += \
    container.h \
    factury.h \
    mainwindow.h \
    ../QRibbon/QRibbon.h \
    sqlhelper.h \
    user.h

FORMS += \
    ../QRibbon/qribbon.ui \
    mainwindow.ui

TRANSLATIONS += \
    QRibbon_yue_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    MainWindow.qrc \
    ../QRibbon/QRibbon.qrc
