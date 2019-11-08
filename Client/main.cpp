#include "mainwindow.h"
#include <QApplication>
#include "login.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    login log;
    log.resize(409,496);
    log.show();
    //MainWindow w;
    //w.show();

    return a.exec();
}
