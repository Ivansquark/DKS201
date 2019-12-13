#include "bd.h"
#include "ui_bd.h"

BD::BD(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BD)
{
    ui->setupUi(this);
}

BD::~BD()
{
    delete ui;
}

void BD::init()
{
    QStringList lst_tabl;
    QSqlDatabase::addDatabase("QSQLITE","myDB1");
    if(QSqlDatabase::contains("myDB1"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB1");
        db.setDatabaseName("/home/pi/mes.db");
        if(!db.open()){qDebug()<<"db was not opened";}
        QSqlQuery query(db);
        lst_tabl=db.tables();
    }
    ui->listWidget->addItems(lst_tabl);
    QFile pass("/home/pi/password1.txt");
    QStringList strList;
    if(pass.exists()&&pass.open(QIODevice::ReadOnly))
    {
        while(!pass.atEnd())
        {
            strList.append(pass.readLine());
        }
        pass.close();
    }
    if (strList[0]==strList[1]||strList[0]==strList[2])
    {
        ui->pushButton_2->setEnabled(true);
    }
    else {ui->pushButton_2->setEnabled(false);ui->pushButton_2->setText("");}
}

void BD::on_pushButton_3_clicked()
{
    close();
}

void BD::on_pushButton_clicked()
{
    if(ui->listWidget->currentItem()!=nullptr)
    {
        BD_table *table = new BD_table(this);
        table->setModal(true);
        table->init(ui->listWidget->currentItem()->text());
        table->showFullScreen();

    }
}
void BD_table::onOkClick()
{

}
void BD_table::onCancelClick()
{
    close();
}

void BD::on_pushButton_2_clicked()//кнопка удалить
{
    if(ui->listWidget->currentItem()!=nullptr)
    {
        QMessageBox msg;
        msg.setModal(true);
        msg.setIcon(QMessageBox::Information);
        QString delTab1="Точно хотите удалить запись: '%1'";
        QString delTab = delTab1.arg(ui->listWidget->currentItem()->text());
        msg.setText(delTab);
        msg.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
        msg.setDefaultButton(QMessageBox::Cancel);
        int ret=msg.exec();
        switch (ret)
        {
            case QMessageBox::Cancel: msg.close();break;
            case QMessageBox::Ok:
            if(QSqlDatabase::contains("myDB1"))
            {
                QSqlDatabase db = QSqlDatabase::database("myDB1");
                db.setDatabaseName("/home/pi/mes.db");
                if(!db.open()){qDebug()<<"db was not opened";}
                QSqlQuery query(db);
                QString queryDelete1 = "DROP TABLE %1";
                QString queryDelete=queryDelete1.arg(ui->listWidget->currentItem()->text());
                if(!query.exec(queryDelete)){qDebug()<<"unable execute query DROP TABLE";}

                QStringList lst_tabl;
                lst_tabl=db.tables();
                ui->listWidget->clear();
                ui->listWidget->addItems(lst_tabl);
            }msg.close(); break;
        }
    }
}
