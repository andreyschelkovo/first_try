#include "server.h"

Server::Server()//11 описание конструктора
{
    if (this->listen(QHostAddress::LocalHost,2323)){//12 если сервер запуст. пишем старт(слушает любой адрес и порт 2323)
        qDebug() << "Start";
    }
    else{
        qDebug() <<"Error";
    }
    nextBlockSize = 0; //-30 обнуляем переменную так же как и в клиенте перед использованием
 }


void Server::incomingConnection(qintptr socketDescriptor){

    socket = new QTcpSocket;//13 создаём новый сокет
    socket->setSocketDescriptor(socketDescriptor);//14 устанавливаем в него дескриптор(неотр число идентефицир номер потока ввода-вывода)
    connect(socket,&QTcpSocket::readyRead,this,&Server::slotReadyRead);//15 объед сигн и слот
    connect(socket,&QTcpSocket::disconnected,socket,&QTcpSocket::deleteLater);//16 при откл сокета, он будет удалён

    Sockets.push_back(socket);//17 для каждого клиента будет создан новый сокет и сохранён в вектор
    qDebug() << "Client connected" << socketDescriptor;//18 вывод сообщения и дескриптор подключённого клиента

}

void Server::slotReadyRead(){

    socket = (QTcpSocket*)sender();//19 запись в переменную именно того сокета с которого пришёл запрос
    QDataStream in(socket);//20 инструмент для приёма передачи данных(класс для работы с потоковым вводом-выводом данных
                                 //обьект in означает ввод, параметром будет наш сокет
    in.setVersion(QDataStream::Qt_6_2);//21 нуэно указать версию (тут хз, свою или какую, я поставил свою)
    if (in.status() == QDataStream::Ok){
        qDebug() << "read...";//22 если ошибок нет, продолжаем работу
//        QString str;//23 так как работа только с текстом, создаем строку и ...
//        in >> str;//24 ... записываем в неё текст из объекта in
//        qDebug() << str; //25 и выведем строку в консоль
//        SendToClient(str);//25+ передаём строку в функцию передачи сообщения клиенту
        for(;;){                                                        //-27.2-
            if(nextBlockSize == 0){                                     //повторяем
                qDebug() << "nextBlockSize == 0";//-28.1доп
                if(socket->bytesAvailable() < 2){                       //эту
                    qDebug() << "Data < 2, Break;";//-28.2доп
                    break;                                              //часть
                }                                                       //как
                in >> nextBlockSize;                                    //и
                qDebug() << "nextBlockSize = " << nextBlockSize;//-28.3доп
            }                                                           //в
            if(socket->bytesAvailable() < nextBlockSize) {              //
                qDebug() << "Data not full,Break";//-28.4доп
                break;                                                  //к
            }                                                           //л
            QString str;                                                //и
            in >> str;                                                  //ен
            nextBlockSize = 0;                                          //те
            qDebug() << str;//-28.5 доп вывод сообщения в консоль
            SendToClient(str);//-27.3- передача строки
            break;//-28- принудительно выходим из цикла
        }
    }
    else{
        qDebug() << "DataStream error";//23 если есть, выводим сообщение об ошибке
    }
}

void Server::SendToClient(QString str){
    Data.clear();//24 чистим массив перед использованием, так как там всегда есть мусор
    QDataStream out(&Data,QIODevice::WriteOnly);//25 созд объект out (вывод) , параметрами будут наш массив байтов и режим работы "только запись"
                                                //хз но типа не будет по другому работать
    out.setVersion(QDataStream::Qt_6_4);//26 снова версия, та же что и в in

    out <<quint16(0) << str;                        //-29копия из клиента--
    out.device()->seek(0);                          //записываем нашу строку в массив байт Data передаём переменную quint16 с параметром 0
    out <<quint16(Data.size() - sizeof(quint16));   //чтобы вместе с переданым сообщением было передано 16 пустых бит и сообщение началось с 17-ого бита
                                                    //идём в начало блока и записываем туда разность размера всего сообщения и переменной quint16

    //out << str;//27 записываем нашу строку в массив байт Data//строку закоментил после второй части видео где копируется часть с клиента

    //socket->write(Data);//28 записываем наш массив байт в сокет

    for(int i = 0; i < Sockets.size(); i++){// -28 записываем наш массив в каждый подключенный сокет
        Sockets[i]->write(Data);
    }


}









