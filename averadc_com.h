#ifndef AVERADC_COM_H
#define AVERADC_COM_H

#include <QDialog>
#include <QDebug>


class AverADC_COM: public QDialog
{
    Q_OBJECT
public:
    AverADC_COM();
    virtual ~AverADC_COM();

signals:
    void sendADC(QString str);
    void sendCOM(QString str);
    void close1();
    void sendTestOff2();

public slots:
    void recieveCOM(QString str);
    void close2();
    //void signalTestOff1();
    //void recieveMesBegin();
    //void recieveEmulBegin();

private:
    QString ADC;
    QString COM;

};

#endif // AVERADC_COM_H
