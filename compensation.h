#ifndef COMPENSATION_H
#define COMPENSATION_H

#include <QDialog>
#include <QTimer>
#include <QDebug>

#include "math.h"

namespace Ui {
class Compensation;
}

class Compensation : public QDialog
{
    Q_OBJECT

public:
    explicit Compensation(QWidget *parent = nullptr);
    ~Compensation();

signals:
    void sendByteToMesPar(QByteArray);
    void compPar(int,int,int);

public slots:
    void recieveADCfromMesPar(QString);
    void realTime();


private slots:
    void on_pushButton_clicked();

private:
    Ui::Compensation *ui;

    QTimer *time=new QTimer(this);
    void delay(int);

    long ADC;
    double ADC_V;
    int Comp1,Comp2,Comp3;
    int T;int y;
};

#endif // COMPENSATION_H
