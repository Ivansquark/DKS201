// Окно Настройки*******************************
#include "setup.h"
#include "ui_setup.h"

Setup::Setup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setup)
{
    ui->setupUi(this);
}
Setup::~Setup()
{
    delete ui;    
}
void Setup::constr(int x)  // функция заменяющая конструктор, при получении определенного сигнала делает кнопки АППАРАТУРА и ТЕСТ недоступными для ОПЕРАТОРА и ПОВЕРИТЕЛЯ
{
    if (x==1)
    {
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
        ui->label_2->setText(strList1[0]);
        ui->label_3->setText(strList1[2]);
        if ((strList1[0]!=strList1[2]))
        {
            ui->pushButton_2->setEnabled(false);
            ui->pushButton_3->setEnabled(false);
            ui->pushButton_2->setText("");
            ui->pushButton_3->setText("");
        }
    }
    if (x==2)
    {
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
        ui->label_2->setText(strList1[0]);
        ui->label_3->setText(strList1[2]);
        if (strList1[0]==strList1[2])
        {
            ui->pushButton_2->setEnabled(true);
            ui->pushButton_3->setEnabled(true);
            ui->pushButton_2->setText("АППАРАТУРА");
            ui->pushButton_3->setText("ТЕСТ");
        }
        if (strList1[0]==strList1[1]) //-запрет ПОВЕРИТЕЛЮ
        {
            ui->pushButton_2->setEnabled(false);
            ui->pushButton_3->setEnabled(false);
            ui->pushButton_2->setText("");
            ui->pushButton_3->setText("");
        }


    }
}
void Setup::on_pushButton_4_clicked()
{
    close();
}

void Setup::on_pushButton_clicked()
{
    commSet= new Setup_commonSetup(this); //-создаем указатель на новое окно по нажатию кнопки а не в конструкторе!!!
    commSet->setModal(true);
    commSet->showFullScreen();
}

//void Setup::closeEvent(QCloseEvent *event)
//{
    //delete commSet; // очищаем память при удалении окна, хз нужно это или память очистится сама(когда? - при закрытии программы, при вызове деструпторов??)
//}

void Setup::on_pushButton_2_clicked()
{
    equipSet=new Setup_equipment(this);
    equipSet->setModal(true);
    equipSet->showFullScreen();
}

void Setup::on_pushButton_3_clicked()
{
    testSet=new Setup_Test(this);
    testSet->setModal(true);
    testSet->showFullScreen();
}
