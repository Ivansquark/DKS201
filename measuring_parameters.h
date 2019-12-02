#ifndef MEASURING_PARAMETERS_H
#define MEASURING_PARAMETERS_H

#include <QDialog>
#include <QSql>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QFile>

#include "compensation.h"


namespace Ui {
class Measuring_parameters;
}

class Measuring_parameters : public QDialog
{
    Q_OBJECT

public:
    explicit Measuring_parameters(QWidget *parent = nullptr);
    ~Measuring_parameters();


    void init();
    void init_CS();

public slots:
    void updateMesuaring_parameters();
    void recieveADCtoMesPar(QString); //функция приема кода с АЦП для компенсации измерения при подключенной камере
    void compPar(int,int,int); //функция принятия сигнала с компенсациями по трем диапазонам.
    void recieveBytesFromCompensation(QByteArray);
    void setCStoMesPar(); // слот приема сигнала о режиме работы с контрольным источником

signals:
    void updateMesPar();
    void sendToPortMes(QByteArray);
    void sendADCfromMesPar(QString);

    void sendBytesToMes(QByteArray);

    void okClick();

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();



private:
    Ui::Measuring_parameters *ui;

    long nullADC;
    long ADC;
    double ADC_V;
    double R1,R2,R3;
};

#endif // MEASURING_PARAMETERS_H
