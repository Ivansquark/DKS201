#ifndef EMULHEX_H
#define EMULHEX_H

#include <QDialog>
#include <QTimer>
#include <QDebug>

#include "emulator.h"
#include "averageadc.h"


class EmulHex: public QDialog
{
    Q_OBJECT
public:
    EmulHex();
    ~EmulHex();
signals:
    void sendCOM(QString str);
    void sendTestOff4();
    void sendMesBegin();
    void sendEmulBegin();
public slots:
    void close1();
    void sendTestOff3();
    void recieveMesBegin();
    void recieveEMUL();

private slots:
    void timeOut();

private:
    QString com="273f4f5f6f7f\0";
    QString sendcom="";
    QTimer *timer=new QTimer(this);

    long iter=0;

};

#endif // EMULHEX_H
