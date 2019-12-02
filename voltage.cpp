#include "voltage.h"
#include "ui_voltage.h"

Voltage::Voltage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Voltage)
{
    ui->setupUi(this);
    ui->radioButton_5->setChecked(true);
}

Voltage::~Voltage()
{
    delete ui;
}

void Voltage::on_pushButton_2_clicked()
{
    close();
    destroy();
}

void Voltage::on_pushButton_3_clicked() // кнопка клавиатуры напряжения задаваемого пользователем
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->showFullScreen();
    connect(this,SIGNAL(clearKeyK(int)),key,SLOT(clearKeyK(int)));
    connect(this,SIGNAL(setOncePoint(int)),key,SLOT(setOncePoint(int)));
    connect(this,SIGNAL(sendString(QString)),key,SLOT(setKeyString(QString)));
    connect(key,SIGNAL(setText(QString)),this,SLOT(setText(QString)));
    emit clearKeyK(4);                                                      // сигнал клавиатуре о цифрах и минусе
    emit sendString(ui->lineEdit->text());
    emit setOncePoint(2);                                                   //сигнал клавиатуре о минусе только как о первом символе
    ui->radioButton->setEnabled(false);
    ui->radioButton_2->setEnabled(false);
    ui->radioButton_3->setEnabled(false);
    ui->radioButton_4->setEnabled(false);
}
void Voltage::setText(QString str)
{
    ui->lineEdit->setText(str);
}

void Voltage::on_pushButton_clicked()  // кнопка ОК
{
    emit sendVolt(ui->lineEdit->text());
    close();
    destroy();
}
void Voltage::recieveVolt(QString str)
{
    if(ui->radioButton_5->isChecked())
    {
        ui->lineEdit->setText(str);
    }
    voltageStr=ui->lineEdit->text();
}

void Voltage::on_radioButton_5_clicked() // радиокнопка - задаваемое пользователем
{
    ui->pushButton_3->setEnabled(true);
}

void Voltage::on_radioButton_clicked() // радиокнопка: +
{
    ui->pushButton_3->setEnabled(false);
    QString strVol=voltageStr;
    if (strVol[0]=="-")
    {
        strVol.remove(0,1);
    }
    ui->lineEdit->setText(strVol);
}

void Voltage::on_radioButton_2_clicked() // радиокнопка: -
{
    ui->pushButton_3->setEnabled(false);
    QString strVol=voltageStr;
    if (strVol[0]!="-")
    {
        strVol="-"+strVol;
    }
    ui->lineEdit->setText(strVol);
}

void Voltage::on_radioButton_3_clicked() // радиокнопка: +1/2
{
    ui->pushButton_3->setEnabled(false);
    QString strVol=voltageStr;
    if (strVol[0]=="-")
    {
        strVol.remove(0,1);
        int strV=strVol.toInt();
        strV=strV/2;
        strVol=QString::number(strV);
    }
    else
    {
        int strV=strVol.toInt();
        strV=strV/2;
        strVol=QString::number(strV);
    }
    ui->lineEdit->setText(strVol);
}

void Voltage::on_radioButton_4_clicked()// радиокнопка: -1/2
{
    ui->pushButton_3->setEnabled(false);
    QString strVol=voltageStr;
    if (strVol[0]!="-")
    {
        strVol="-"+strVol;
        int strV=strVol.toInt();
        strV=strV/2;
        strVol=QString::number(strV);
    }
    else
    {
        int strV=strVol.toInt();
        strV=strV/2;
        strVol=QString::number(strV);
    }
    ui->lineEdit->setText(strVol);
}
