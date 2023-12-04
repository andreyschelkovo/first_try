#include "mainwindow.h"

#include <QApplication>
//some line for commit
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
