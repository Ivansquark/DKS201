// Окно Настройки-общие настройки***************************

#include "setup_commonsetup.h"
#include "ui_setup_commonsetup.h"

Setup_commonSetup::Setup_commonSetup(QWidget *parent) : //конструктор, т.к. создаем объект по кнопке он в зависимости от информации из файлов конструируется по разному
    QDialog(parent),
    ui(new Ui::Setup_commonSetup)
{
    ui->setupUi(this);


    QFile password("/home/pi/password1.txt");
    QStringList strList1;
    if ((password.exists())&&password.open(QIODevice::ReadOnly))
    {
        while(!password.atEnd())
        {
            strList1.append(password.readLine());
        }
        password.close();
    }
    ui->label_3->setText(strList1[0]);
    ui->label_4->setText(strList1[2]);


    if (strList1[0]==strList1[2])
    {
        ui->pushButton->setEnabled(true);
        ui->pushButton->setText("Список\nпользователей");
    }
    else
    {
        ui->pushButton->setEnabled(false);
        ui->pushButton->setText("");
    }

    QFile languageR("/home/pi/language.txt");
    QByteArray lang;
    if ((languageR.exists())&&languageR.open(QIODevice::ReadOnly))
    {
        while (!languageR.atEnd())
        {
            lang.append(languageR.readLine());
        }
        languageR.close();
    }
    if (lang=="1")
    {
        ui->radioButton->setChecked(true);
        ui->label_5->setText(lang);
    }
    if (lang=="2")
    {
        ui->radioButton_2->setChecked(true);
        ui->label_5->setText(lang);
    }

    QFile timeR("/home/pi/time.txt");
    QByteArray time;
    if ((timeR.exists())&&timeR.open(QIODevice::ReadOnly))
    {
        while (!timeR.atEnd())
        {
            time.append(timeR.readLine());
        }
        timeR.close();
    }
    if (time=="1")
    {
        ui->radioButton_3->setChecked(true);
        ui->label_6->setText(time);
    }
    if (time=="2")
    {
        ui->radioButton_4->setChecked(true);
        ui->label_6->setText(time);
    }
}

Setup_commonSetup::~Setup_commonSetup()
{
    delete ui;
    delete userlist;
}

void Setup_commonSetup::on_pushButton_2_clicked()
{
    close();
}

void Setup_commonSetup::on_pushButton_clicked()
{
    userlist = new Setup_CommonSetup_UserList(this);
    userlist->setModal(true);
    userlist->showFullScreen();
}


void Setup_commonSetup::on_radioButton_2_clicked() // функция записи в файл по нажатию RadioButton
{
    QFile languageW("/home/pi/language.txt");
    QByteArray langW;
    if ((languageW.exists())&&(languageW.open(QIODevice::WriteOnly)))
    {
        QTextStream stream(&languageW);
        langW="2"; // соответствует Русский;
        stream<<langW;
    }
    ui->label_5->setText(langW);
}

void Setup_commonSetup::on_radioButton_clicked() // функция записи в файл по нажатию RadioButton
{
    QFile languageW("/home/pi/language.txt");
    QByteArray langW;
    QTextStream stream(&languageW);
    if ((languageW.exists())&&languageW.open(QIODevice::WriteOnly))
    {
        langW="1"; // соответствует Русский;
        stream<<langW;
    }
    ui->label_5->setText(langW);
}

void Setup_commonSetup::on_radioButton_3_clicked() // функция записи в файл по нажатию RadioButton
{
    QFile timeW("/home/pi/time.txt");
    QByteArray time;
    QTextStream stream(&timeW);
    if ((timeW.exists())&&timeW.open(QIODevice::WriteOnly))
    {
        time="1"; // соответствует часы,минуты,секунды;
        stream<<time;
    }
    ui->label_6->setText(time);
}

void Setup_commonSetup::on_radioButton_4_clicked() // функция записи в файл по нажатию RadioButton
{
    QFile timeW("/home/pi/time.txt");
    QByteArray time;
    QTextStream stream(&timeW);
    if ((timeW.exists())&&timeW.open(QIODevice::WriteOnly))
    {
        time="2"; // соответствует секунды;
        stream<<time;
    }
    ui->label_6->setText(time);
}
