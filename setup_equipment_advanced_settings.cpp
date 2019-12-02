#include "setup_equipment_advanced_settings.h"
#include "ui_setup_equipment_advanced_settings.h"

Setup_Equipment_Advanced_Settings::Setup_Equipment_Advanced_Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setup_Equipment_Advanced_Settings)
{
    ui->setupUi(this);

    QFile advC("/home/pi/advconfig.txt");
    QStringList strList;
    if (advC.exists()&&advC.open(QIODevice::ReadOnly))
    {
        while (!advC.atEnd())
        {
            strList.append(advC.readLine());
        }
        advC.close();
    }
    QString str1=strList[0];
    str1.chop(1);
    QString str2=strList[1];
    str2.chop(1);
    QString str3=strList[2];
    str3.chop(1);
    QString str4=strList[3];
    str4.chop(1);
    QString str5=strList[4];
    str5.chop(1);
    QString str6=strList[5];
    str6.chop(1);
    QString str7=strList[6];
    str7.chop(1);
    ui->lineEdit->setText(str1);
    ui->lineEdit_2->setText(str2);
    ui->lineEdit_3->setText(str3);
    ui->lineEdit_4->setText(str4);
    ui->lineEdit_5->setText(str5);
    ui->lineEdit_6->setText(str6);
    ui->lineEdit_7->setText(str7);
}

Setup_Equipment_Advanced_Settings::~Setup_Equipment_Advanced_Settings()
{
    delete ui;
}

void Setup_Equipment_Advanced_Settings::on_pushButton_clicked()
{
    close();
}

void Setup_Equipment_Advanced_Settings::setText1(QString str)
{
    ui->lineEdit->setText(str);

    QFile advC("/home/pi/advconfig.txt");
    QStringList strList;
    QTextStream stream(&advC);
    if (advC.exists()&&advC.open(QIODevice::ReadOnly))
    {
        while (!advC.atEnd())
        {
            strList.append(advC.readLine());
        }
        advC.close();
    }
    strList[0]=ui->lineEdit->text()+"\n";
    if (advC.exists()&&advC.open(QIODevice::WriteOnly))
    {
        for (QString str:strList)
        {
            stream<<str;
        }
        advC.close();
    }
}
void Setup_Equipment_Advanced_Settings::setText2(QString str)
{
    ui->lineEdit_2->setText(str);

    QFile advC("/home/pi/advconfig.txt");
    QStringList strList;
    QTextStream stream(&advC);
    if (advC.exists()&&advC.open(QIODevice::ReadOnly))
    {
        while (!advC.atEnd())
        {
            strList.append(advC.readLine());
        }
        advC.close();
    }
    strList[1]=ui->lineEdit_2->text()+"\n";
    if (advC.exists()&&advC.open(QIODevice::WriteOnly))
    {
        for (QString str:strList)
        {
            stream<<str;
        }
        advC.close();
    }
}
void Setup_Equipment_Advanced_Settings::setText3(QString str)
{
    ui->lineEdit_3->setText(str);

    QFile advC("/home/pi/advconfig.txt");
    QStringList strList;
    QTextStream stream(&advC);
    if (advC.exists()&&advC.open(QIODevice::ReadOnly))
    {
        while (!advC.atEnd())
        {
            strList.append(advC.readLine());
        }
        advC.close();
    }
    strList[2]=ui->lineEdit_3->text()+"\n";
    if (advC.exists()&&advC.open(QIODevice::WriteOnly))
    {
        for (QString str:strList)
        {
            stream<<str;
        }
        advC.close();
    }
}
void Setup_Equipment_Advanced_Settings::setText4(QString str)
{
    ui->lineEdit_4->setText(str);

    QFile advC("/home/pi/advconfig.txt");
    QStringList strList;
    QTextStream stream(&advC);
    if (advC.exists()&&advC.open(QIODevice::ReadOnly))
    {
        while (!advC.atEnd())
        {
            strList.append(advC.readLine());
        }
        advC.close();
    }
    strList[3]=ui->lineEdit_4->text()+"\n";
    if (advC.exists()&&advC.open(QIODevice::WriteOnly))
    {
        for (QString str:strList)
        {
            stream<<str;
        }
        advC.close();
    }
}
void Setup_Equipment_Advanced_Settings::setText5(QString str)
{
    ui->lineEdit_5->setText(str);

    QFile advC("/home/pi/advconfig.txt");
    QStringList strList;
    QTextStream stream(&advC);
    if (advC.exists()&&advC.open(QIODevice::ReadOnly))
    {
        while (!advC.atEnd())
        {
            strList.append(advC.readLine());
        }
        advC.close();
    }
    strList[4]=ui->lineEdit_5->text()+"\n";
    if (advC.exists()&&advC.open(QIODevice::WriteOnly))
    {
        for (QString str:strList)
        {
            stream<<str;
        }
        advC.close();
    }
}
void Setup_Equipment_Advanced_Settings::setText6(QString str)
{
    ui->lineEdit_6->setText(str);

    QFile advC("/home/pi/advconfig.txt");
    QStringList strList;
    QTextStream stream(&advC);
    if (advC.exists()&&advC.open(QIODevice::ReadOnly))
    {
        while (!advC.atEnd())
        {
            strList.append(advC.readLine());
        }
        advC.close();
    }
    strList[5]=ui->lineEdit_6->text()+"\n";
    if (advC.exists()&&advC.open(QIODevice::WriteOnly))
    {
        for (QString str:strList)
        {
            stream<<str;
        }
        advC.close();
    }
}
void Setup_Equipment_Advanced_Settings::setText7(QString str)
{
    ui->lineEdit_7->setText(str);

    QFile advC("/home/pi/advconfig.txt");
    QStringList strList;
    QTextStream stream(&advC);
    if (advC.exists()&&advC.open(QIODevice::ReadOnly))
    {
        while (!advC.atEnd())
        {
            strList.append(advC.readLine());
        }
        advC.close();
    }
    strList[6]=ui->lineEdit_7->text()+"\n";
    if (advC.exists()&&advC.open(QIODevice::WriteOnly))
    {
        for (QString str:strList)
        {
            stream<<str;
        }
        advC.close();
    }
}


void Setup_Equipment_Advanced_Settings::on_pushButton_2_clicked()
{
    keyboard *key1 = new keyboard(this);
    connect(this,SIGNAL(setOncePoint(int)),key1,SLOT(setOncePoint(int)));
    connect(this,SIGNAL(setKeyString(QString)),key1,SLOT(setKeyString(QString)));
    connect(this,SIGNAL(ClearKey(int)),key1,SLOT(clearKeyK(int)));
    connect(key1,SIGNAL(setText(QString)),this,SLOT(setText1(QString)));

    key1->setModal(true);
    key1->showFullScreen();

    emit ClearKey(2);
    emit setKeyString(ui->lineEdit->text());
    emit setOncePoint(1);

}

void Setup_Equipment_Advanced_Settings::on_pushButton_7_clicked()
{
    keyboard *key2 = new keyboard(this);
    connect(this,SIGNAL(setOncePoint(int)),key2,SLOT(setOncePoint(int)));
    connect(this,SIGNAL(setKeyString(QString)),key2,SLOT(setKeyString(QString)));
    connect(this,SIGNAL(ClearKey(int)),key2,SLOT(clearKeyK(int)));
    connect(key2,SIGNAL(setText(QString)),this,SLOT(setText2(QString)));

    key2->setModal(true);
    key2->showFullScreen();

    emit ClearKey(2);
    emit setKeyString(ui->lineEdit_2->text());
    emit setOncePoint(1);
}

void Setup_Equipment_Advanced_Settings::on_pushButton_3_clicked()
{
    keyboard *key3 = new keyboard(this);
    connect(this,SIGNAL(setOncePoint(int)),key3,SLOT(setOncePoint(int)));
    connect(this,SIGNAL(setKeyString(QString)),key3,SLOT(setKeyString(QString)));
    connect(this,SIGNAL(ClearKey(int)),key3,SLOT(clearKeyK(int)));
    connect(key3,SIGNAL(setText(QString)),this,SLOT(setText3(QString)));

    key3->setModal(true);
    key3->showFullScreen();

    emit ClearKey(2);
    emit setKeyString(ui->lineEdit_3->text());
    emit setOncePoint(1);
}

void Setup_Equipment_Advanced_Settings::on_pushButton_4_clicked()
{
    keyboard *key4 = new keyboard(this);
    connect(this,SIGNAL(setOncePoint(int)),key4,SLOT(setOncePoint(int)));
    connect(this,SIGNAL(setKeyString(QString)),key4,SLOT(setKeyString(QString)));
    connect(this,SIGNAL(ClearKey(int)),key4,SLOT(clearKeyK(int)));
    connect(key4,SIGNAL(setText(QString)),this,SLOT(setText4(QString)));

    key4->setModal(true);
    key4->showFullScreen();

    emit ClearKey(2);
    emit setKeyString(ui->lineEdit_4->text());
    emit setOncePoint(1);
}

void Setup_Equipment_Advanced_Settings::on_pushButton_6_clicked()
{
    keyboard *key5 = new keyboard(this);
    connect(this,SIGNAL(setOncePoint(int)),key5,SLOT(setOncePoint(int)));
    connect(this,SIGNAL(setKeyString(QString)),key5,SLOT(setKeyString(QString)));
    connect(this,SIGNAL(ClearKey(int)),key5,SLOT(clearKeyK(int)));
    connect(key5,SIGNAL(setText(QString)),this,SLOT(setText5(QString)));

    key5->setModal(true);
    key5->showFullScreen();

    emit ClearKey(2);
    emit setKeyString(ui->lineEdit_5->text());
    emit setOncePoint(1);
}

void Setup_Equipment_Advanced_Settings::on_pushButton_5_clicked()
{
    keyboard *key6 = new keyboard(this);
    connect(this,SIGNAL(setOncePoint(int)),key6,SLOT(setOncePoint(int)));
    connect(this,SIGNAL(setKeyString(QString)),key6,SLOT(setKeyString(QString)));
    connect(this,SIGNAL(ClearKey(int)),key6,SLOT(clearKeyK(int)));
    connect(key6,SIGNAL(setText(QString)),this,SLOT(setText6(QString)));

    key6->setModal(true);
    key6->showFullScreen();

    emit ClearKey(2);
    emit setKeyString(ui->lineEdit_6->text());
    emit setOncePoint(1);
}

void Setup_Equipment_Advanced_Settings::on_pushButton_8_clicked()
{
    keyboard *key7 = new keyboard(this);
    connect(this,SIGNAL(setOncePoint(int)),key7,SLOT(setOncePoint(int)));
    connect(this,SIGNAL(setKeyString(QString)),key7,SLOT(setKeyString(QString)));
    connect(this,SIGNAL(ClearKey(int)),key7,SLOT(clearKeyK(int)));
    connect(key7,SIGNAL(setText(QString)),this,SLOT(setText7(QString)));

    key7->setModal(true);
    key7->showFullScreen();

    emit ClearKey(2);
    emit setKeyString(ui->lineEdit_7->text());
    emit setOncePoint(1);
}
