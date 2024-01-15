#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    nickname = "anon";

    clientsocket = new QTcpSocket(this);//4 инициализируем сокет
    connect(clientsocket,&QTcpSocket::readyRead,this,&MainWindow::slotReadyRead);//5 подкл так же как и на сервере (но с клиент сокетом)
    connect(clientsocket,&QTcpSocket::disconnected,clientsocket,&QTcpSocket::deleteLater);//6 -//-
    //connect(this,&MainWindow::nicknane_changeCL_sig,this, &MainWindow::ni )
    nextBlockSize = 0;//-29 обнуляем переменную перед использованием
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_connect_btn_clicked()
{
    clientsocket->connectToHost("127.0.0.1",2323);//6+ подключаемся к серверу , адрес локальный, порт тот же что в сервере
    if (clientsocket->state()){
        ui->textBrowser->append("connection completed successfully");
    }

}

void MainWindow::SendToServer(QString str)      //14 тут один в один как на сервере ток с клиент сокетом
{

    Data.clear();                               //чистим массив перед использованием, так как там всегда есть мусор
    QDataStream out(&Data,QIODevice::WriteOnly);//созд объект out (вывод) , параметрами будут наш массив байтов и режим работы "только запись"
                                                //хз но типа не будет по другому работать
    out.setVersion(QDataStream::Qt_6_2);        // снова версия, та же что и в in
    QDateTime date;
    date = date.currentDateTime();
    out <<quint16(0) << date << nickname << str; // записываем нашу строку в массив байт Data ---17--- передаём переменную quint16 с параметром 0
                                                 //-17-чтобы вместе с переданым сообщением было передано 16 пустых бит и сообщение началось с 17-ого бита
    out.device()->seek(0); //-17-идём в начало блока и...
    out <<quint16(Data.size() - sizeof(quint16));//...записываем туда разность размера всего сообщения и переменной quint16
    clientsocket->write(Data);                  // записываем наш массив байт в сокет
    ui->lineEdit->clear();//очищаем строку ввода текста, после отправки сообщения
}


void MainWindow::slotReadyRead()
{
    QDataStream in(clientsocket);//7 описание как на сервере (но с клиент сокетом)
    in.setVersion(QDataStream::Qt_6_4);// 8 -//-

    if(in.status() == QDataStream::Ok){//9 если соединение ок ...
//        QString str;//10 создаём строку...
//        in>>str;//11 передаём масив в строку...-------------------------------------------18 коментирую эти 3 строки
//        ui->textBrowser->append(str);//12 выводим строку в текстовое окно клиента
        for(;;){//-19-в бесконечном цикле..
            if(nextBlockSize == 0){//...считываем блок если его размер неизвестен (=0),...
               if(clientsocket->bytesAvailable() < 2){//...или если для чтения доступно не меньше 2х байт(16бит)
                    break;
                }
                in >> nextBlockSize;//-20- если условия выполняются, считываем размер блока
            }
            if(clientsocket->bytesAvailable() < nextBlockSize) {//-21- сравниваем размер блока с размером байт, которые пришли от сервера
                break;//-22-если размер блока болше, то данные пришли не полностью, выходим из цикла
            }
            QString str;//-23-если мы еще внутри цикла, то создаём строку...
            QDateTime date;


            in >> date >> nickname >> str ; //-24- считываем в неё данные //102 добавляем в поток время
            nextBlockSize = 0;// -25- обнуляем размер блока для обработки нового сообщения
            ui->textBrowser->append(date.toString("dd.MM.yyyy"));
            ui->textBrowser->append(nickname + ": " + str);//-26- выводим строку в текстовое окно клиента //103 отображаем время в интерфейсе
        }
    }else {
        ui->textBrowser->append("Error");//13 при ошибке туда же печатаем ошибку
    }
}

void MainWindow::on_send_btn_clicked()
{

    SendToServer(ui->lineEdit->text());//15 вызов функции отправки сообщ на сервер и отрисовка его в окне чата по клику на кнопку
}

//123325
void MainWindow::on_lineEdit_returnPressed()
{

    SendToServer(ui->lineEdit->text());//16 -//- но по нажатию энтер в строке ввода
}


void MainWindow::on_pushButton_your_name_clicked()
{
     nickname = ui->lineEdit_name->text();
     ui->textBrowser->append("The name is changed");
     ui->lineEdit_name->clear();

}


void MainWindow::on_lineEdit_name_returnPressed()
{
    nickname = ui->lineEdit_name->text();
    ui->textBrowser->append("The name is changed");
    ui->lineEdit_name->clear();
}

