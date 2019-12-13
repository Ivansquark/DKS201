#ifndef MEASURING_H
#define MEASURING_H

#include <QDialog>
#include <QSql>
#include <QSqlQuery>
#include <QDebug>
#include <QTimer>
#include <QFile>
#include <QMessageBox>
#include <QtMath>
#include <QDate>

#include "measuring_mode.h"
#include "measurement_config.h"
#include "measuring_parameters.h"
#include "sources.h"

namespace Ui {
class Measuring;
}

class Measuring : public QDialog
{
    Q_OBJECT

public:
    explicit Measuring(QWidget *parent = nullptr);
    ~Measuring();

signals:
    void close2();        // сигнал о закрытии эмулятора
    void changeRangeSIG();
    void closePortTest(); // сигнал о закрытии порта
    void bytesVoltM(QByteArray);
    void sendToPortMainW(QByteArray data);// сигнал о посылке в порт команды о включении доп фильтра (конденсатора С1- 560пФ)
    void threshold_stop();
    void sendADCtoMesPar(QString);
    void setCStoMesPar();


public slots:
    void recieveStr(QString);
    void updateMes();
    void updateMesPar();
    void recieveADC(QString);
    void changeRangeSLOT();
    void recieveADC_COM(QString str);
    void bytesVolt1(QByteArray);
    void sendToPortMes(QByteArray); //прием сигнала из окна MesPar о включении доп фильтра
    void stopClick();  //функция приема сигнала о достижении порога и нажатия на клавишу стоп
    void sendBytesFromMes(QByteArray); //функция отправки в порт байтов от окна компенсации утечек с камерой
    void setCS(QString); // функция приема сигнала о включении режима контрольного источника.



private slots:
    void on_pushButton_6_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_7_clicked();

    void timeOut();

    void on_pushButton_8_clicked();

    void on_pushButton_4_clicked();





private:
    Ui::Measuring *ui;
    QTimer *timer=new QTimer(this);
    int Time_threshold;
    double DOSE_threshold;
    long avrADC;
    int time;
    long nullADC;
    double ADC_V;
    double C1,C2,R1,R2,R3,inI,inU;
    double integral_I,integral_DoseRate;
    double Q,D;                           //заряд и доза

    double AverageMes, StandartDeviation; //
    QString standart_deviation_show;      // Для СКО
    double av,otkl;                 //

    double pressure, temperature, correctionF, sensibility;
    QString measuring_parameter_dimension, dimensionSensibility,measuring_parameter,measurement_range;

    void displaingMes_label_7(double); //отображение информации в виде 0.000 (0000)
    void displaingMes_label_10(double); //отображение информации в виде 0.000 (0000)
    void displaingMes_label_12(double z);
    void displaingMes_label_14(double z);//CKO

    double compensation_ADC_V(); // функция учитывающая выполненную компенсацию по диапазонам

    bool CSbut;
    double doseRateCurrentCS;
    double doseRateVerificationCS;
    QDate dateVerificationCS;
    double half_lifeCS;
    QString doseRateDimension;
    double doseRateFull;

    bool CS_flag;

    QString mesDB_currentName; //имя текущей таблицы в БД mes.db

    void mesToBD(); // создание таблицы в БД mes.bd , таблица создается при запуске измерения.
    void mesToTableBD(long second, double value); //заполнение созданной таблицы измеренными значениями ()
};

#endif // MEASURING_H
