#include <QCoreApplication>
#include "server.h" //29 подключаем хедер

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server s;//30 создаём объект нашего класса сервер
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("./testBD.db");
    if (db.open()){
        qDebug ("db open");
    }
    else{
        qDebug ("db no open");
    }

    return a.exec();
}
