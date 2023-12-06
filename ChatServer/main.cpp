#include <QCoreApplication>
#include "server.h" //29 подключаем хедер

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server s;//30 создаём объект нашего класса сервер

    return a.exec();
}
