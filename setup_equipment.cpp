#include "setup_equipment.h"
#include "ui_setup_equipment.h"

Setup_equipment::Setup_equipment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setup_equipment)
{
    ui->setupUi(this);

    QFile config("/home/pi/config.txt");
    QStringList strList;
    if (config.exists()&&config.open(QIODevice::ReadOnly))
    {
        while(!config.atEnd())
        {
            strList.append(config.readLine());
        }
    }
    if (strList[0]=="1\n")
    {
        ui->radioButton->setChecked(true);
    }
    if (strList[0]=="2\n")
    {
        ui->radioButton_2->setChecked(true);
    }
    QString str1=strList[1];
    QString str2=strList[2];
    QString str3=strList[3];
    str1.chop(1);
    str2.chop(1);
    str3.chop(1);
    ui->lineEdit->setText(str1);
    ui->lineEdit_2->setText(str2);
    ui->lineEdit_3->setText(str3); // записываем строку в lineEdit без последнего символа;
}

Setup_equipment::~Setup_equipment()
{
    delete ui;
}

void Setup_equipment::on_changed() {
  double result;
  result = ui->lineEdit->text().toDouble();
  ui->lineEdit_2->setText(QString::number(result));
}


void Setup_equipment::on_pushButton_clicked()
{
    close();
}

void Setup_equipment::on_pushButton_5_clicked()
{
    setEqAS =new Setup_Equipment_Advanced_Settings(this);
    setEqAS->setModal(true);
    setEqAS->showFullScreen();
}

void Setup_equipment::on_radioButton_clicked()
{
    QFile config("/home/pi/config.txt");
    QStringList strList;
    QTextStream stream(&config);

    if(config.exists()&&config.open(QIODevice::ReadOnly))
    {
        while(!config.atEnd())
        {
            strList.append(config.readLine());
        }
        strList[0]="1\n";
        config.close();
    }
    if (config.exists()&&config.open(QIODevice::WriteOnly))
    {
        foreach(QString str,strList)
        {
            stream<<str;
        }
        config.close();
    }
}

void Setup_equipment::on_radioButton_2_clicked()
{
    QFile config("/home/pi/config.txt");
    QStringList strList;
    QTextStream stream(&config);

    if(config.exists()&&config.open(QIODevice::ReadOnly))
    {
        while(!config.atEnd())
        {
            strList.append(config.readLine());
        }
        strList[0]="2\n";
        config.close();
    }
    if (config.exists()&&config.open(QIODevice::WriteOnly))
    {
        foreach(QString str,strList)
        {
            stream<<str;
        }
        config.close();
    }
}

void Setup_equipment::on_pushButton_2_clicked()
{
    key1=new keyboard(this);

    connect(this,SIGNAL(setKey1(int)),key1,SLOT(clearKeyK(int)));
    connect(key1,SIGNAL(setText(QString)),this,SLOT(setText1(QString)));

    connect(this,SIGNAL(setKeyString(QString)),key1,SLOT(setKeyString(QString)));

    connect(this,SIGNAL(setOncePoint(int)),key1,SLOT(setOncePoint(int)));

    key1->setModal(true);
    key1->showFullScreen();
    emit setKey1(2);
    emit setKeyString(ui->lineEdit->text());
    emit setOncePoint(1);
}

void Setup_equipment::on_pushButton_3_clicked()
{
    key2=new keyboard(this);
    connect(this,SIGNAL(setKey1(int)),key2,SLOT(clearKeyK(int)));
    connect(key2,SIGNAL(setText(QString)),this,SLOT(setText2(QString)));

    connect(this,SIGNAL(setKeyString(QString)),key2,SLOT(setKeyString(QString)));

    connect(this,SIGNAL(setOncePoint(int)),key2,SLOT(setOncePoint(int)));

    key2->setModal(true);
    key2->showFullScreen();
    emit setKey1(2);
    emit setKeyString(ui->lineEdit_2->text());
    emit setOncePoint(1);
}

void Setup_equipment::on_pushButton_4_clicked()
{

    key3=new keyboard(this);
    connect(this,SIGNAL(setKey1(int)),key3,SLOT(clearKeyK(int)));
    connect(key3,SIGNAL(setText(QString)),this,SLOT(setText3(QString)));

    connect(this,SIGNAL(setKeyString(QString)),key3,SLOT(setKeyString(QString)));

    connect(this,SIGNAL(setOncePoint(int)),key3,SLOT(setOncePoint(int)));

    key3->setModal(true);
    key3->showFullScreen();
    emit setKey1(2);
    emit setKeyString(ui->lineEdit_3->text());
    emit setOncePoint(1);
}

void Setup_equipment::setText1(QString str)
{
    ui->lineEdit->setText(str);


    QFile config("/home/pi/config.txt");
    QStringList strList;
    QTextStream stream(&config);
    if (config.exists()&&config.open(QIODevice::ReadOnly))
    {
        while(!config.atEnd())
        {
            strList.append(config.readLine());
        }
        config.close();
    }
    strList[1]=ui->lineEdit->text()+"\n";
    if(config.exists()&&config.open(QIODevice::WriteOnly))
    {
        foreach(QString str1,strList)
        {
            stream<<str1;
        }
    }
}
void Setup_equipment::setText2(QString str)
{
    ui->lineEdit_2->setText(str);
    QFile config("/home/pi/config.txt");
    QStringList strList;
    QTextStream stream(&config);
    if (config.exists()&&config.open(QIODevice::ReadOnly))
    {
        while(!config.atEnd())
        {
            strList.append(config.readLine());
        }
        config.close();
    }
    strList[2]=ui->lineEdit_2->text()+"\n";
    if(config.exists()&&config.open(QIODevice::WriteOnly))
    {
        foreach(QString str1,strList)
        {
            stream<<str1;
        }
    }
}
void Setup_equipment::setText3(QString str)
{
    ui->lineEdit_3->setText(str);
    QFile config("/home/pi/config.txt");
    QStringList strList;
    QTextStream stream(&config);
    if (config.exists()&&config.open(QIODevice::ReadOnly))
    {
        while(!config.atEnd())
        {
            strList.append(config.readLine());
        }
        config.close();
    }
    strList[3]=ui->lineEdit_3->text()+"\n";
    if(config.exists()&&config.open(QIODevice::WriteOnly))
    {
        foreach(QString str1,strList)
        {
            stream<<str1;
        }
    }
}












