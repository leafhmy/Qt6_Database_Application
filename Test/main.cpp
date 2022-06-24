#include "mainwindow.h"
#include "vgribbonwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    VGRibbonWindow w;
    MainWindow w;
    w.setWindowTitle("你卷你妈呢");
    w.showFullScreen();
    return a.exec();
}
