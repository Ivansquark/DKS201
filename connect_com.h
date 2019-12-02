#ifndef CONNECT_COM_H
#define CONNECT_COM_H

#include <QDialog>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QTimer>
#include <QThread>

#include <com.h>


namespace Ui {
class Connect_com;
}

class Connect_com : public QDialog
{
    Q_OBJECT

public:
    explicit Connect_com(QWidget *parent = nullptr);
    ~Connect_com();

signals:
    void timerStop();
    void writeData(QByteArray);

public slots:

    void print(QString);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_10_clicked();

private:
    Ui::Connect_com *ui;
    QString COM;
    QSerialPort *current_serial = new QSerialPort(this);
    void SortCOM(QSerialPort*);

    QTimer *timer = new QTimer(this);
    QThread *thread_New = new QThread(this);//Создаем поток для COM порта
    Com *PortNew = new Com(this);//Создаем обьект по классу COM порта (считывает данные по таймеру, и передает асинхронно в порт команды)
};

#endif // CONNECT_COM_H
