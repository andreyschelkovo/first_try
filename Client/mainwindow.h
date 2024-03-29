#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>//1 подкл библ сокет
#include <QTcpServer>
#include <QTime> // 100 подключаем бибблиотеку для отображения тамй меток с сообщениями
#include <QMessageBox>
#include <QDateTime>

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
    void on_pushButton_your_name_clicked();

    void on_lineEdit_name_returnPressed();

private:

    Ui::MainWindow *ui;
    QTcpSocket *clientsocket; //2 создаём клиентский сокет
    QByteArray Data;// 3 создаём массив байтов
    void SendToServer(QString str); //14...
    quint16 nextBlockSize;//-17- целое положительное число размером 16 бит для контроля объёма полученной информации
    QString nickname;
public slots:
    void slotReadyRead();//7...

signals:
    void nicknane_changeCL_sig();


};
#endif // MAINWINDOW_H
