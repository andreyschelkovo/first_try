#ifndef SERVER_H
#define SERVER_H
//#include <QObject>
#include <QTcpServer>//1добавили библиотеки тсп сервера и сокета
#include <QTcpSocket>
#include <QVector>// 5 подключаем библиотеку для создания векторов
#include <QTime> // 104 для сервера повторяем тоже самое
#include <QSqlDatabase>
#include <QDebug>

class Server : public QTcpServer//2унаследовали от ктсп сервера
{
    Q_OBJECT // 3 для использования сигнально-слотовых связей
public:
    Server();
    QTcpSocket *socket; //4 создаём поле-экземпляр сокета
private:
    QVector <QTcpSocket*> Sockets;//6 создаём векто указателей ктсп-сокетов для хранения.НОвый сокет будет создан при каждом подкючении
    QByteArray Data;// 7 тип данных(массив байтов) который будет передаваться между сервером и клиентом
    void SendToClient(QString str);//8 объявляю функцию для передачи данных клиенту
    quint16 nextBlockSize;//-27-повторяем как в клиенте

public slots:
    void incomingConnection(qintptr socketDescriptor);//9ф для обр-ки новю подкл.
    void slotReadyRead();//10 ф. слот для сигнала. обр-чик полученных от кл. сообщ.

};
///try to see this file
#endif // SERVER_H
