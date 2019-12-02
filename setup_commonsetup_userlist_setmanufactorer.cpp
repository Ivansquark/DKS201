// Окно Настройки-общие настройки-список пользователей-изготовитель***************************


#include "setup_commonsetup_userlist_setmanufactorer.h"
#include "ui_setup_commonsetup_userlist_setmanufactorer.h"


Setup_CommonSetup_UserList_SetManufactorer::Setup_CommonSetup_UserList_SetManufactorer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setup_CommonSetup_UserList_SetManufactorer)
{
    ui->setupUi(this);
    key1=new keyboard(this);
    key2=new keyboard(this);

    connect(key1,SIGNAL(setText(QString)),this,SLOT(setLine1(QString)));
    connect(key2,SIGNAL(setText(QString)),this,SLOT(setLine2(QString)));

    QFile password("/home/pi/password1.txt");

    if ((password.exists())&&password.open(QIODevice::ReadOnly))
    {
        QStringList strList;
        while (!password.atEnd())
        {
            strList.append(password.readLine());
        }
        ui->lineEdit->setText(strList[2]);
        ui->lineEdit_2->setText(strList[2]);
    }
}

Setup_CommonSetup_UserList_SetManufactorer::~Setup_CommonSetup_UserList_SetManufactorer()
{
    delete ui;
}

void Setup_CommonSetup_UserList_SetManufactorer::on_pushButton_3_clicked()
{
    close();
}

void Setup_CommonSetup_UserList_SetManufactorer::on_pushButton_2_clicked()
{
    key1->setModal(true);
    key1->showFullScreen();
}

void Setup_CommonSetup_UserList_SetManufactorer::on_pushButton_clicked()
{
    key2->setModal(true);
    key2->showFullScreen();
}
void Setup_CommonSetup_UserList_SetManufactorer::setLine1(QString str)
{
    ui->lineEdit->setText(str);
    ui->lineEdit->setEchoMode(QLineEdit::Password);
    ui->lineEdit_2->setText("");
}
void Setup_CommonSetup_UserList_SetManufactorer::setLine2(QString str)
{
    ui->lineEdit_2->setText(str);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
}

void Setup_CommonSetup_UserList_SetManufactorer::on_pushButton_4_clicked()
{
    if ((ui->lineEdit->text()==ui->lineEdit_2->text())&&ui->lineEdit->text()!="")
    {
        QMessageBox::information(nullptr,"Внимание!!!","Пароль был установлен");
        QFile password("/home/pi/password1.txt");
        QStringList strList;
        if ((password.exists())&&(password.open(QIODevice::ReadOnly)))
        {

                while (!password.atEnd())
                {
                    strList<<password.readLine();
                }

                password.close();
        }
         strList[2]=ui->lineEdit->text()+"\n"; // присваивает новый пароль третьей строке в файле.


        if ((password.exists())&&(password.open(QIODevice::WriteOnly)))
        {
            QTextStream stream (&password);
            foreach(QString str, strList)
            {
               //QByteArray array;
               //array.append(str); // преобразует строку QString в массив символов QByteArray
               //QTextCodec *Codec1251 = QTextCodec::codecForName("Windows-1251");//объявление указателя на объект QTextCodec, конвертирующего из chcp1251 в unicode
               //QString ResultUnicode = Codec1251->toUnicode(array); //конвертирует в юникод
               //QTextCodec *CodecUtf8=QTextCodec::codecForName("UTF-8"); //объявление объекта QTextCodec, конвертирующего из UNICODE в utf-8
               //QString strUTF8=CodecUtf8->fromUnicode(ResultUnicode); // конвертирует в UTF-8

               //stream<<strUTF8;
                stream<<str;
            }
            password.close();
        }

    }
    else QMessageBox::warning(nullptr,"Внимание!!!","Контроль пароля не пройден\nПароли не совпадают");

}
