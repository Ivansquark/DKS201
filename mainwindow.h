//Главное окно

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QThread>

#include "setup.h"
#include "keyboard.h"
#include "emulhex.h"
#include "measuring_popup.h"
#include "chambers.h"
#include "measurement_config.h"
#include "measuring.h"
#include "measuring_parameters.h"
#include "connect_com.h"
#include "averadc_com.h"

#include "date.h"
#include "bd.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Exit_clicked();
    void on_NewWin_clicked();
    void on_pushButton_clicked();


    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();



    void on_pushButton_8_clicked();

public slots:
    void setText1(QString str);
    void recieveTestOff();
    void writeToComRecieve(QByteArray data); // - принимаемая команда по которой осществляется передача в порт

    void DisconnectPortRecieve();
    void print(QString);            // - строка из СОМ порта в MainWindow
    void recieveADC(QString);
    void process_Port_Start();  //старт СОМ порта в потоке (переопределение Run)
    void recieveTestBegin();
    void sendToComFromTest(QByteArray data);
    void testEnd();             // функция окончания теста от СОМ порта
    void closePortTest(); //функция приема сигнала от окна теста закрывающая порт (чтобы можно было заново запустить измерение)   
    void repeatTest(); // функция приема сигнала от кнопки повторить тест
    void okTest(); // функция приема сигнала от кнопки ok теста
    void okChambers(); //функция приема сигнала от кнопки ok камер впервые
    void okMesConf(); // сигнал о нажатии ок впервые для взова следующего окна
    void bytesVolt(QByteArray data); //слот принятия данных о текущем высоком напряжении
    void bytesVoltM(QByteArray); // слот сигнала с байтами высокого напряжения из окна измерения (во второй раз)
    void sendToPortMainW(QByteArray);


signals:    
    void clearKeyM(int x);
    void constrSET(int);
    void sendMesBegin();
    void sendEMUL();
    void ConnectPort();
    void DisconnectPort();
    void writeToCom(QByteArray data); // - команда в Порт
    void COMsend(QString); // - Строка с СОМ порта из MainWindow
    void sendEmulBegin(); // - сигнал о начале теста
    void sendTestBegin(); // - сигнал о начале теста
    void sendADCdec(QString);// -  Строка ADC с СОМ порта из MainWindow в test
    void close2(); // сигнал о закрытии averADC
    void writeData(QByteArray); // посылаем данные в порт
    void closePort();
    void terminateThread(); //сигнал о закрытии порта
    void firstTimeFlag(); // сигнал окну с выбором камере о первом измерении
    void mesCOM();        // сигнал о вызове окна измерения от СОМ порта

public:

private:
    Ui::MainWindow *ui;
    Setup *setup;
    keyboard *key;
    int C;

};

#endif // MAINWINDOW_H
