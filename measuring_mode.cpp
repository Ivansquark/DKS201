#include "measuring_mode.h"
#include "ui_measuring_mode.h"

Measuring_mode::Measuring_mode(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Measuring_mode)
{
    ui->setupUi(this);
    ui->radioButton_4->setChecked(true);
    ui->lineEdit_7->setEnabled(false);
    ui->pushButton_8->setEnabled(false);
    QSqlDatabase::addDatabase("QSQLITE","myDB");
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("/home/pi/chambers.db");
        db.open();
        QSqlQuery query(db);
        query.exec("SELECT * FROM config");
        if(query.first())
        {
            ui->lineEdit_7->setText(query.value("measurement_automode_times").toString());
        }
        db.close();
    }
}

Measuring_mode::~Measuring_mode()
{
    delete ui;
}

void Measuring_mode::on_pushButton_2_clicked()
{
    close();
}

void Measuring_mode::on_radioButton_4_clicked() //радиокнопка ручного режима
{
    if(ui->radioButton_4->isChecked())
    {
        ui->lineEdit_7->setEnabled(false);
        ui->pushButton_8->setEnabled(false);
    }
}

void Measuring_mode::on_radioButton_5_clicked() //радиокнопка автоматического режима
{
    if(ui->radioButton_5->isChecked())
    {
        ui->lineEdit_7->setEnabled(true);
        ui->pushButton_8->setEnabled(true);
    }
}

void Measuring_mode::on_pushButton_clicked() // кнопка ОК
{
    if(ui->radioButton_4->isChecked())
    {
        emit sendStr("Ручной режим");
        if(QSqlDatabase::contains("myDB"))
        {
            QSqlDatabase db=QSqlDatabase::database("myDB");
            db.setDatabaseName("/home/pi/chambers.db");
            db.open();
            QSqlQuery query(db);
            QString update = "UPDATE config SET measurement_mode='%1'";
            QString update1 = update.arg("manual");
            if(!query.exec(update1))
            {
                qDebug()<<"unable execute query manual";
            }
            db.close();
        }
    }
    if(ui->radioButton_5->isChecked())
    {
        emit sendStr("Автоматический режим\n количество измерений: "+ui->lineEdit_7->text());
        if(QSqlDatabase::contains("myDB"))
        {
            QSqlDatabase db=QSqlDatabase::database("myDB");
            db.setDatabaseName("/home/pi/chambers.db");
            db.open();
            QSqlQuery query(db);
            QString update="UPDATE config SET measurement_mode='%1', measurement_automode_times='%2';";
            QString update1=update.arg("auto").arg(ui->lineEdit_7->text());
            if(!query.exec(update1))
            {
                qDebug()<<"unable execute query auto";
            }
            db.close();
        }
    }
    close();
}

void Measuring_mode::on_pushButton_8_clicked() // кнопка клавиатуры
{
    keyboard *key= new keyboard(this);
    key->setModal(true);
    key->showFullScreen();

    connect(this,SIGNAL(clearKeyK(int)),key,SLOT(clearKeyK(int)));
    connect(this,SIGNAL(sendString(QString)),key,SLOT(setKeyString(QString)));
    connect(key,SIGNAL(setText(QString)),this,SLOT(setText(QString)));
    emit clearKeyK(3);
    emit sendString(ui->lineEdit_7->text());
}
void Measuring_mode::setText(QString str)
{
    ui->lineEdit_7->setText(str);
}
