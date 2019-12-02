#include "date.h"
#include "ui_date.h"

Date::Date(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Date)
{
    ui->setupUi(this);
}

Date::~Date()
{
    delete ui;
}

void Date::on_pushButton_2_clicked()
{
    keyboard *key=new keyboard(this);
    connect(this,SIGNAL(setKeyString(QString )),key,SLOT(setKeyString(QString)));
    connect(key,SIGNAL(setText(QString)),this,SLOT(setText1(QString)));
    connect(this,SIGNAL(setOncePoint(int)),key,SLOT(setOncePoint(int)));

    emit setKeyString(ui->lineEdit->text());
    emit setOncePoint(4);

    key->setModal(true);
    key->showFullScreen();
}
void Date::setText1(QString str)
{
    ui->lineEdit->setText(str);
}

void Date::on_pushButton_3_clicked()
{
    keyboard *key=new keyboard(this);
    connect(this,SIGNAL(setKeyString(QString )),key,SLOT(setKeyString(QString)));
    connect(key,SIGNAL(setText(QString)),this,SLOT(setText2(QString)));
    connect(this,SIGNAL(setOncePoint(int)),key,SLOT(setOncePoint(int)));

    emit setKeyString(ui->lineEdit_2->text());
    emit setOncePoint(4);

    key->setModal(true);
    key->showFullScreen();
}
void Date::setText2(QString str)
{
    ui->lineEdit_2->setText(str);
}

void Date::on_pushButton_4_clicked()
{
    keyboard *key=new keyboard(this);
    connect(this,SIGNAL(setKeyString(QString )),key,SLOT(setKeyString(QString)));
    connect(key,SIGNAL(setText(QString)),this,SLOT(setText3(QString)));
    connect(this,SIGNAL(setOncePoint(int)),key,SLOT(setOncePoint(int)));

    emit setKeyString(ui->lineEdit_3->text());
    emit setOncePoint(4);

    key->setModal(true);
    key->showFullScreen();
}
void Date::setText3(QString str)
{
    ui->lineEdit_3->setText(str);
}

void Date::on_pushButton_clicked()
{
    close();
}

void Date::on_pushButton_9_clicked() //кнопка ОК
{
    int day=0,month=0,year=0;
    day=ui->lineEdit->text().toInt();
    month=ui->lineEdit_2->text().toInt();
    year=ui->lineEdit_3->text().toInt();
    qDebug()<<day<<month<<year;
    if (day<=0||day>31)
    {
        QMessageBox mb;
        mb.setModal(true);
        mb.setIcon(QMessageBox::Information);
        QString strMsg = "Неправильное число";
        mb.setText(strMsg);
        mb.setStandardButtons(QMessageBox::Ok);
        mb.setDefaultButton(QMessageBox::Ok);
        mb.showFullScreen();
        mb.exec();
        qDebug()<<day;

    }
    else if(month<1||month>12)
    {
        QMessageBox mb;
        mb.setModal(true);
        mb.setIcon(QMessageBox::Information);
        QString strMsg = "Неправильный месяц";
        mb.setText(strMsg);
        mb.setStandardButtons(QMessageBox::Ok);
        mb.setDefaultButton(QMessageBox::Ok);
        mb.showFullScreen();
        mb.exec();
        qDebug()<<month;
    }
    else if(year<2015||year>2050)
    {
        QMessageBox mb;
        mb.setModal(true);
        mb.setIcon(QMessageBox::Information);
        QString strMsg = "Неправильный год";
        mb.setText(strMsg);
        mb.setStandardButtons(QMessageBox::Ok);
        mb.setDefaultButton(QMessageBox::Ok);
        mb.showFullScreen();
        mb.exec();
        qDebug()<<year;
    }
    else
    {
        date=QDate(year,month,day);
        qDebug()<<date;
        if (date.isValid())
        {
            qDebug()<<"OK";
            //qDebug()<<date.toString(Qt::LocalDate);
            emit sendDate(day,month,year);
            close();
        }
        else
        {
            qDebug()<<"NO";
            QMessageBox mb;
            mb.setModal(true);
            mb.setIcon(QMessageBox::Information);
            QString strMsg = "Неправильная дата";
            mb.setText(strMsg);
            mb.setStandardButtons(QMessageBox::Ok);
            mb.setDefaultButton(QMessageBox::Ok);
            mb.showFullScreen();
            mb.exec();
            qDebug()<<date.toString(Qt::LocalDate);
        }
    }
}
