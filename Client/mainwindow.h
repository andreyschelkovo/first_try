#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>//1 подкл библ сокет
#include <QTime> // 100 подключаем бибблиотеку для отображения тамй меток с сообщениями

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_connect_btn_clicked();//5 ...
    void on_send_btn_clicked();
    void on_lineEdit_returnPressed();//16 для отправки по нажатию энтер
private:
    Ui::MainWindow *ui;
    QTcpSocket *socket; //2 создаём клиентский сокет
    QByteArray Data;// 3 создаём массив байтов
    void SendToServer(QString str); //14...
    quint16 nextBlockSize;//-17- целое положительное число размером 16 бит для контроля объёма полученной информации
public slots:
    void slotReadyRead();//7...

};
#endif // MAINWINDOW_H
