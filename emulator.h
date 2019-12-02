#ifndef EMULATOR_H
#define EMULATOR_H

#include <QDialog>
#include <QTime>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "measuring_popup.h"

namespace Ui {
class Emulator;
}

class Emulator : public QDialog
{
    Q_OBJECT

public:
    explicit Emulator(QWidget *parent = nullptr);
    ~Emulator();

    long timeoutTEST=0;
    long timeREF=0;
signals:
    void close2();
    void signalTestOff();


public slots:
    void recieveADC(QString str);
    void recieveCOM(QString str);




private slots:
    void on_pushButton_clicked();

    void slotTimerAlarm();    

    void sredneeADC_V();

private:
    Ui::Emulator *ui;
    long i=0,x=0,y=0,l1=0,l2=0;
    long l0 = 8388608; // - константа соответствующая разряду АЦП при нулевом напряжении;
    double ADC_V=0,d=0;

    int warmingTime=0;
    int timeoutC2=0;
    int timeoutTest=0;
    int timeoutI=0;
    int Kintegral=0; // интеграция отсчетов Т=К*период опроса порта;
    int averNum=0;
    double R1=0,R2=0,R3=0,C2=0,I=0;
    double DAC;

    QString strADC="";


};

#endif // EMULATOR_H
