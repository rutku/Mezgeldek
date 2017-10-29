#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;
    int h = 300;
    int w = 250;
    window.setGeometry(500,100,w,h);
    window.setFixedSize(w,h);
    window.setAcceptDrops(true);
    window.show();

    return a.exec();
}
