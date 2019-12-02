#include "compensation.h"
#include "ui_compensation.h"

Compensation::Compensation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Compensation)
{
    ui->setupUi(this);
    ui->progressBar->setValue(0);
    T=0;
    y=0;
    Comp1=0;Comp2=0;Comp3=0;
    ADC=0;
    connect(time,SIGNAL(timeout()),this,SLOT(realTime()));
    time->start(1000);
}

Compensation::~Compensation()
{
    delete ui;
    //delete time;
    qDebug()<<"destructor compensation";
}

void Compensation::delay(int x)
{
    while (x>0) x--;
}

void Compensation::recieveADCfromMesPar(QString str)
{
    ui->label_2->setText(str);
    ADC=str.toLong();
}

void Compensation::realTime()
{
    T+=1;
    if (T==1)
    {
        ui->label->setText("Измерения на чувствительном диапазоне");
        QByteArray h82;
        h82[0]=40;//28
        h82[1]=50;//32 код реле - К1+К8 - Чувствительный диапазон
        emit sendByteToMesPar(h82);
        delay(10000);
        emit sendByteToMesPar(h82);
        ui->progressBar->setValue(0);
    }
    if (T>=2 && T<=16)
    {
         ui->progressBar->setValue(10 + 5*(T-2));
    }

    if (T>=16 && T<=20)
    {
        ui->progressBar->setValue(10 + 5*(T-2));

        y+=ADC;
        qDebug()<<y;
        if (T==20)
        {
           ADC_V=y/5;
           Comp1=static_cast<int>(round(ADC_V));
           qDebug()<<"Comp1"<<Comp1;
           y=0;
        }
    }

    if (T==21)
    {
        ui->label->setText("Измерения на среднем диапазоне");
        QByteArray h90;
        h90[0]=0x29;//29
        h90[1]=0x30;//30 код реле - К3+К8 - Средний диапазон (на среднем диапазоне фильтр шумов не подключается)
        emit sendByteToMesPar(h90);
        delay(10000);
        emit sendByteToMesPar(h90);
        ui->progressBar->setValue(0);
    }
    if (T>=22 && T<=36)
    {
         ui->progressBar->setValue(10 + 5*(T-22));
    }
    if (T>=36 && T<=40)
    {
        ui->progressBar->setValue(10 + 5*(T-22));

        y+=ADC;
        qDebug()<<y;
        if (T==40)
        {
           ADC_V=y/5;
           Comp2=static_cast<int>(round(ADC_V));
           qDebug()<<"Comp2"<<Comp2;
           y=0;
        }
    }

    if (T==41)
    {
        ui->label->setText("Измерения на грубом диапазоне");
        QByteArray hA0;
        hA0[0]=0x2a;//2a
        hA0[1]=0x30;//30 код реле - К5+К7+К8 - Грубый диапазон + фильтр шумов (С1 - 560пФ) (на грубом диапазоне фильтр шумов не отключается)
        emit sendByteToMesPar(hA0);
        delay(10000);
        emit sendByteToMesPar(hA0);
        ui->progressBar->setValue(0);
    }
    if (T>=42 && T<=56)
    {
         ui->progressBar->setValue(10 + 5*(T-42));
    }

    if (T>=56 && T<=60)
    {
        ui->progressBar->setValue(10 + 5*(T-42));

        y+=ADC;
        qDebug()<<y;
        if (T==60)
        {
           ADC_V=y/5;
           Comp3=static_cast<int>(round(ADC_V));
           qDebug()<<"Comp3"<<Comp3;
           y=0;
        }
    }
    if (T==61) ui->progressBar->setValue(100);
    if (T==62)
    {
        close();
        emit compPar(Comp1,Comp2,Comp3);
        delete this;
    }
    qDebug()<<T;
}

void Compensation::on_pushButton_clicked()
{
    close();
    delete this;
}
