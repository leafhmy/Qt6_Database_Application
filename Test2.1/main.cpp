#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("你卷你妈呢");
    w.showFullScreen();
    return a.exec();
}
