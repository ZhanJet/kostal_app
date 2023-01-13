#include "mainwindow.h"
#include "mainwindowtest.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
//    MainWindowTest w1;
//    w1.show();

    MainWindow w;
    w.show();

    return a.exec();
}
