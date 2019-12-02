#ifndef AVERAGEADC_H
#define AVERAGEADC_H

#include <QDialog>
#include <QTimer>
#include <QFile>
#include <QDebug>

#include "emulator.h"
#include "measuring.h"
#include "testcom.h"


class AverageADC: public QDialog
{
    Q_OBJECT
public:
    AverageADC();
    virtual ~AverageADC();

signals:
    void sendADC(QString str);
    void sendCOM(QString str);
    void close1();
    void sendTestOff2();

public slots:
    void recieveCOM(QString str);
    void TimeOut();
    void close2();
    void signalTestOff1();
    void recieveMesBegin();
    void recieveEmulBegin();

private:
    Emulator *test;
    QTimer *timer;
    QString ADC="";
    QString COM="";

    int requestPeriod=0;
};

#endif // AVERAGEADC_H
