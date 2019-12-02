#include "add_source.h"
#include "ui_add_source.h"

Add_source::Add_source(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Add_source)
{
    ui->setupUi(this);

}

Add_source::~Add_source()
{
    delete ui;
}

void Add_source::init1()
{
    QSqlDatabase::addDatabase("QSQLITE","MyDB");
    QFile pass("/home/pi/password1.txt");
    QStringList strList;
    if (pass.exists()&&pass.open(QIODevice::ReadOnly))
    {
        while(!pass.atEnd())
        {
            strList.append(pass.readLine());
        }
        pass.close();
    }
    if (strList[0]==strList[2])
    {
        ui->pushButton_9->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_5->setEnabled(true);
        ui->pushButton_6->setEnabled(true);
        ui->pushButton_7->setEnabled(true);
        ui->pushButton_10->setEnabled(true);
        ui->pushButton_11->setEnabled(true);

        ui->pushButton_9->setText("Добавить");

    }
    else if(strList[0]==strList[1])
    {
        ui->pushButton_9->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_5->setEnabled(false);
        ui->pushButton_6->setEnabled(true);
        ui->pushButton_7->setEnabled(true);
        ui->pushButton_10->setEnabled(false);
        ui->pushButton_11->setEnabled(false);
    }
    else
    {
        ui->pushButton_9->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_5->setEnabled(false);
        ui->pushButton_6->setEnabled(false);
        ui->pushButton_7->setEnabled(false);
        ui->pushButton_10->setEnabled(false);
        ui->pushButton_11->setEnabled(false);
    }

    ui->label_2->setText(dateCurrent.currentDate().toString(Qt::LocalDate));
    //QString ex = strList[4];
    //qDebug()<<"ex";
    ui->comboBox_2->addItem("мкГр/с");
    ui->comboBox_2->addItem("мГр/с");
    ui->comboBox_2->addItem("мкЗв/с");
    ui->comboBox_2->addItem("мЗв/с");
    //ui->comboBox_2->addItem("мкР/с");
    //ui->comboBox_2->addItem("мкР/с");
}

void Add_source::init2()
{
    QSqlDatabase::addDatabase("QSQLITE","MyDB");
    QFile pass("/home/pi/password1.txt");
    QStringList strList;
    if (pass.exists()&&pass.open(QIODevice::ReadOnly))
    {
        while(!pass.atEnd())
        {
            strList.append(pass.readLine());
        }
        pass.close();
    }
    if (strList[0]==strList[2])
    {
        ui->pushButton_9->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_5->setEnabled(true);
        ui->pushButton_6->setEnabled(true);
        ui->pushButton_7->setEnabled(true);
        ui->pushButton_10->setEnabled(true);
        ui->pushButton_11->setEnabled(true);

        ui->pushButton_9->setText("Исправить");
    }
    else if(strList[0]==strList[1])
    {
        ui->pushButton_9->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_5->setEnabled(false);
        ui->pushButton_6->setEnabled(true);
        ui->pushButton_7->setEnabled(true);
        ui->pushButton_10->setEnabled(false);
        ui->pushButton_11->setEnabled(false);
    }
    else
    {
        ui->pushButton_9->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_5->setEnabled(false);
        ui->pushButton_6->setEnabled(false);
        ui->pushButton_7->setEnabled(false);
        ui->pushButton_10->setEnabled(false);
        ui->pushButton_11->setEnabled(false);
    }

    ui->label_2->setText(dateCurrent.currentDate().toString(Qt::LocalDate));
    //QString ex = strList[4];
    qDebug()<<"ex";
    ui->comboBox_2->addItem("мкГр/с");
    ui->comboBox_2->addItem("мГр/с");
    ui->comboBox_2->addItem("мкЗв/с");
    ui->comboBox_2->addItem("мЗв/с");
    //ui->comboBox_2->addItem("мкР/с");
    //ui->comboBox_2->addItem("мкР/с");
}

void Add_source::on_pushButton_clicked()
{
    close();
}

void Add_source::on_pushButton_2_clicked() //название источника
{
    keyboard *key = new keyboard(this);
    connect(this,SIGNAL(clearKeyK(int)),key,SLOT(clearKeyK(int)));
    connect(this,SIGNAL(setKeyString(QString)),key,SLOT(setKeyString(QString)));
    connect(key,SIGNAL(setText(QString)),this,SLOT(setTextLine_1(QString)));

    key->setModal(true);
    key->showFullScreen();
    //emit clearKeyK(3);
    emit setKeyString(ui->lineEdit->text());
}
void Add_source::setTextLine_1(QString str)
{
    ui->lineEdit->setText(str);
}

void Add_source::on_pushButton_10_clicked() //заводской номер
{
    keyboard *key = new keyboard(this);
    connect(this,SIGNAL(clearKeyK(int)),key,SLOT(clearKeyK(int)));
    connect(this,SIGNAL(setKeyString(QString)),key,SLOT(setKeyString(QString)));
    connect(key,SIGNAL(setText(QString)),this,SLOT(setTextLine_2(QString)));
    key->setModal(true);
    key->showFullScreen();
    //emit clearKeyK(1);
    emit setKeyString(ui->lineEdit_2->text());
}
void Add_source::setTextLine_2(QString str)
{
    ui->lineEdit_8->setText(str);
}

void Add_source::on_pushButton_3_clicked() // дата изготовления
{
    Date *date = new Date(this);
    connect(date,SIGNAL(sendDate(int,int,int)),this,SLOT(recieveDate1(int,int,int)));
    date->setModal(true);
    date->showFullScreen();
}
void Add_source::recieveDate1(int day,int month, int year)
{
    QDate date(year,month,day);
    ui->lineEdit_2->setText(date.toString(Qt::LocalDate));
    dateFabrication=date;
    //long long i = QDate::currentDate().toJulianDay()-date.toJulianDay();

}

void Add_source::on_pushButton_5_clicked() // период полураспада
{
    keyboard *key = new keyboard(this);
    connect(this,SIGNAL(clearKeyK(int)),key,SLOT(clearKeyK(int)));
    connect(this,SIGNAL(setKeyString(QString)),key,SLOT(setKeyString(QString)));
    connect(key,SIGNAL(setText(QString)),this,SLOT(setTextLine_3(QString)));
    key->setModal(true);
    key->showFullScreen();
    emit clearKeyK(3);
    emit setKeyString(ui->lineEdit_4->text());
}
void Add_source::setTextLine_3(QString str)
{
    ui->lineEdit_4->setText(str);
    half_life=str.toDouble();
}

void Add_source::on_pushButton_11_clicked() // активность начальная, МБк
{
    keyboard *key = new keyboard(this);
    connect(this,SIGNAL(clearKeyK(int)),key,SLOT(clearKeyK(int)));
    connect(this,SIGNAL(setKeyString(QString)),key,SLOT(setKeyString(QString)));
    connect(key,SIGNAL(setText(QString)),this,SLOT(setTextLine_4(QString)));
    key->setModal(true);
    key->showFullScreen();
    emit clearKeyK(3);
    emit setKeyString(ui->lineEdit_9->text());
}
void Add_source::setTextLine_4(QString str)
{
    ui->lineEdit_9->setText(str);
    activityFabrication=str.toDouble();
    activityCurrent=activityFabrication*pow(2,-1*(QDate::currentDate().toJulianDay()-dateFabrication.toJulianDay())/(365*half_life));
    ui->label_7->setText(QString::number(activityCurrent));
}

void Add_source::on_pushButton_4_clicked() // изотоп (по русски)
{
    keyboard *key = new keyboard(this);
    connect(this,SIGNAL(clearKeyK(int)),key,SLOT(clearKeyK(int)));
    connect(this,SIGNAL(setKeyString(QString)),key,SLOT(setKeyString(QString)));
    connect(key,SIGNAL(setText(QString)),this,SLOT(setTextLine_5(QString)));
    key->setModal(true);
    key->showFullScreen();
    //emit clearKeyK(3);
    emit setKeyString(ui->lineEdit_3->text());
}
void Add_source::setTextLine_5(QString str)
{
    ui->lineEdit_3->setText(str);
}

void Add_source::on_pushButton_6_clicked() // дата последней поверки
{
    Date *date = new Date(this);
    connect(date,SIGNAL(sendDate(int,int,int)),this,SLOT(recieveDate2(int,int,int)));
    date->setModal(true);
    date->showFullScreen();
}
void Add_source::recieveDate2(int day,int month, int year)
{
    QDate date(year,month,day);
    ui->lineEdit_5->setText(date.toString(Qt::LocalDate));
    dateVerification=date;
}


void Add_source::on_pushButton_7_clicked() //мощность при последней поверке
{
    keyboard *key = new keyboard(this);
    connect(this,SIGNAL(clearKeyK(int)),key,SLOT(clearKeyK(int)));
    connect(this,SIGNAL(setKeyString(QString)),key,SLOT(setKeyString(QString)));
    connect(key,SIGNAL(setText(QString)),this,SLOT(setTextLine_6(QString)));
    key->setModal(true);
    key->showFullScreen();
    emit clearKeyK(2);
    emit setKeyString(ui->lineEdit_6->text());
}
void Add_source::setTextLine_6(QString str)
{
    ui->lineEdit_6->setText(str);
    doseRateVerification=str.toDouble();


    doseRateCurrent=doseRateVerification*pow(2,-1*(QDate::currentDate().toJulianDay()-dateVerification.toJulianDay())/(365*half_life));

    ui->label_16->setText(QString::number(doseRateCurrent));
}

void Add_source::on_pushButton_9_clicked() // кнопка добавить (исправить)
{
    if(ui->pushButton_9->text()=="Добавить")
    {
        if (QSqlDatabase::contains("myDB"))
        {
            QSqlDatabase db=QSqlDatabase::database("myDB");
            db.setDatabaseName("/home/pi/chambers.db");
            if (!db.open())
            {
                qDebug()<<"db was not open";
            }
            QSqlQuery query(db);
            QString strCreate = "CREATE TABLE IF NOT EXISTS sources ("
                        "number TEXT(10),"
                        "sourceName TEXT(10),"
                        "sorceNumber TEXT(10),"
                        "sourceDateManufactory TEXT(10),"
                        "isotope TEXT(10),"
                        "half_life TEXT(10),"
                        "activityManufactory TEXT(10),"
                        "sourceDateVerification TEXT(10),"
                        "doseRateVerification TEXT(10),"
                        "doseRateDimension TEXT(10);";
            if(!query.exec(strCreate))
            {
                qDebug()<<"Unable create table";
            }

            if (!query.exec("SELECT * FROM sources"))
            {
            qDebug()<<"Unable execute query";
            }

            int num=0;
            if(query.last()!=0)
            {
                 num=query.value("number").toInt();
            }
            num+=1;
            QString strNum=QString::number(num);
            qDebug()<<strNum;
            QString insert =
            "INSERT INTO sources (number, sourceName,sorceNumber,sourceDateManufactory,isotope, half_life, activityManufactory, sourceDateVerification, doseRateVerification,doseRateDimension) "
            "VALUES ('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10');";
            QString insert1 = insert.arg(strNum)
                        .arg(ui->lineEdit->text()) // sourceName
                        .arg(ui->lineEdit_8->text()) // sorceNumber
                        .arg(ui->lineEdit_2->text()) // sourceDateManufactory
                        .arg(ui->lineEdit_3->text()) // isotope
                        .arg(ui->lineEdit_4->text()) // half_life
                        .arg(ui->lineEdit_9->text()) // activityManufactory
                        .arg(ui->lineEdit_5->text()) // sourceDateVerification
                        .arg(ui->lineEdit_6->text()) // doseRateVerification
                        .arg(ui->comboBox_2->currentText()); // doseRateDimension
            if (!query.exec(insert1))
            {
                qDebug() << "Unable to do insert opeation";
            }
        }
    }
    if(ui->pushButton_9->text()=="Исправить")
    {
        if(QSqlDatabase::contains("myDB"))
        {
            QSqlDatabase db = QSqlDatabase::database("myDB");
            db.setDatabaseName("/home/pi/chambers.db");
            if(!db.open())
            {
                qDebug()<<"db was not open";
            }
            QSqlQuery query(db);
            query.prepare("UPDATE sources SET sourceName=:sourceName, sorceNumber=:sorceNumber, sourceDateManufactory=:sourceDateManufactory,isotope=:isotope,"
                          "half_life=:half_life,activityManufactory=:activityManufactory,sourceDateVerification=:sourceDateVerification,"
                          "doseRateVerification=:doseRateVerification,doseRateDimension=:doseRateDimension"
                          " WHERE number=:number");
            query.bindValue(":sourceName",ui->lineEdit->text());
            query.bindValue(":sorceNumber",ui->lineEdit_8->text());
            query.bindValue(":sourceDateManufactory",ui->lineEdit_2->text());
            query.bindValue(":isotope",ui->lineEdit_3->text());
            query.bindValue(":half_life",ui->lineEdit_4->text());
            query.bindValue(":activityManufactory",ui->lineEdit_9->text());
            query.bindValue(":sourceDateVerification",ui->lineEdit_5->text());
            query.bindValue(":doseRateVerification",ui->lineEdit_6->text());
            query.bindValue(":doseRateDimension",ui->comboBox_2->currentText());
            query.bindValue(":number",ui->label_11->text());
            if(!query.exec())
            {
                qDebug()<<"Was not updated";
            }
        }
    }
    emit newSourceList();
    close();
}
void Add_source::constr(QString num) // передача сигнала о выставлении значений в форме по базе данных
{
    //ui->pushButton_9->setText("Исправить");
    int numInt=num.toInt();
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("/home/pi/chambers.db");
        if(!db.open())
        {
            qDebug()<<"db was not open";
        }
        QSqlQuery query(db);
        if(!query.exec("SELECT * FROM sources"))
        {
            qDebug()<<"unable execute query!!!";
        }
        while(query.next())
        {
            QString numStr = query.value("number").toString();
            if(numInt==numStr.toInt())
            {
                ui->lineEdit->setText(query.value("sourceName").toString());
                ui->lineEdit_8->setText(query.value("sorceNumber").toString());
                ui->lineEdit_2->setText(query.value("sourceDateManufactory").toString());
                ui->lineEdit_3->setText(query.value("isotope").toString());
                ui->lineEdit_4->setText(query.value("half_life").toString());
                ui->lineEdit_9->setText(query.value("activityManufactory").toString());
                ui->lineEdit_5->setText(query.value("sourceDateVerification").toString());
                ui->lineEdit_6->setText(query.value("doseRateVerification").toString());
                ui->comboBox_2->setCurrentText(query.value("doseRateDimension").toString());

                ui->label_11->setText(query.value("number").toString());
            }
        }
    }
}
