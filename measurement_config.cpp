#include "measurement_config.h"
#include "ui_measurement_config.h"

Measurement_config::Measurement_config(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Measurement_config)
{
    F=0;
    ui->setupUi(this);
    QSqlDatabase::addDatabase("QSQLITE","myDB");
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("/home/pi/chambers.db");
        if(!db.open())
        {
            qDebug()<<"db was not opened";
        }
        QSqlQuery query(db);
        if(!query.exec("SELECT * FROM config"))
        {
            qDebug()<<"unable execute query";
        }
        if(query.first())
        {
            ui->label_2->setText(query.value("name").toString());
            ui->label_3->setText(query.value("voltage").toString());
            ui->lineEdit->setText(query.value("temperature").toString());
            ui->lineEdit_2->setText(query.value("pressure").toString());
            ui->lineEdit_4->setText(query.value("correction_factor").toString());
            ui->radioButton_2->setText("по дозе "+query.value("dimensionSensibility").toString());
            qDebug()<<query.value("threshold").toString();
            if(query.value("threshold").toString()=="no")  // выставление радиокнопки в состояние no при загрузке
            {
                ui->radioButton->setChecked(true);
                ui->lineEdit_5->setEnabled(false);
                ui->lineEdit_6->setEnabled(false);
            }

            if(query.value("threshold").toString()=="dose")
            {
                ui->radioButton_2->setChecked(true);
                ui->lineEdit_6->setEnabled(false);
                ui->lineEdit_5->setText(query.value("threshold_dose").toString());
            }
            if(query.value("threshold").toString()=="time")
            {
                ui->radioButton_3->setChecked(true);
                ui->lineEdit_5->setEnabled(false);
                ui->lineEdit_6->setText(query.value("threshold_time").toString());
            }
        }
    }
    double x= ui->lineEdit_2->text().toDouble()*101.3/760;
    ui->lineEdit_3->setText(QString::number(x,10,3));

}

Measurement_config::~Measurement_config()
{
    delete ui;
}
void Measurement_config::updateData() //функция обновления формы при приеме сигнала о смене камеры
{
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("/home/pi/chambers.db");
        if(!db.open())
        {
            qDebug()<<"db was not opened";
        }
        QSqlQuery query(db);
        if(!query.exec("SELECT * FROM config"))
        {
            qDebug()<<"unable execute query";
        }
        if(query.first())
        {
            ui->label_2->setText(query.value("name").toString());
            ui->label_3->setText(query.value("voltage").toString());
            ui->lineEdit->setText(query.value("temperature").toString());
            ui->lineEdit_2->setText(query.value("pressure").toString());
            ui->lineEdit_4->setText(query.value("correction_factor").toString());
            ui->radioButton_2->setText("по дозе "+query.value("dimensionSensibility").toString());
        }
    }
}
void Measurement_config::on_pushButton_clicked() // кнопка выбор камеры
{
    Chambers *choose = new Chambers(this);
    choose->setModal(true);
    choose->showFullScreen();
    connect(choose,SIGNAL(updateData()),this,SLOT(updateData()));
}

void Measurement_config::on_pushButton_2_clicked() // кнопка напряжение
{
    Voltage *vol = new Voltage(this);
    vol->setModal(true);
    vol->showFullScreen();
    connect(this,SIGNAL(sendVolt(QString)),vol,SLOT(recieveVolt(QString)));
    connect(vol,SIGNAL(sendVolt(QString)),this,SLOT(recieveVolt(QString)));
    emit sendVolt(ui->label_3->text());
}
void Measurement_config::recieveVolt(QString str) //функция получения напряжения из окна выставления напряжения
{
    ui->label_3->setText(str);
}

void Measurement_config::on_pushButton_10_clicked()
{
    close();    
}

void Measurement_config::on_pushButton_3_clicked() //кнопка клавиатуры температуры
{
    keyboard *key = new keyboard(this);
    key->setModal(true);
    key->showFullScreen();
    connect(this,SIGNAL(clearKeyK(int)),key,SLOT(clearKeyK(int)));
    connect(this,SIGNAL(setOncePoint(int)),key,SLOT(setOncePoint(int)));
    connect(this,SIGNAL(sendString(QString)),key,SLOT(setKeyString(QString)));
    connect(key,SIGNAL(setText(QString)),this,SLOT(setTextTemp(QString)));
    emit clearKeyK(2);
    emit sendString(ui->lineEdit->text());
    emit setOncePoint(1);

}
void Measurement_config::setTextTemp(QString str) // функция приема сигнала от клавиатуры температуры и обновления значения в БД config
{
    ui->lineEdit->setText(str);
}


void Measurement_config::on_pushButton_4_clicked() // кнопка клавиатуры давления
{
    keyboard *key = new keyboard(this);
    key->setModal(true);
    key->showFullScreen();
    connect(this,SIGNAL(clearKeyK(int)),key,SLOT(clearKeyK(int)));
    connect(this,SIGNAL(setOncePoint(int)),key,SLOT(setOncePoint(int)));
    connect(this,SIGNAL(sendString(QString)),key,SLOT(setKeyString(QString)));
    connect(key,SIGNAL(setText(QString)),this,SLOT(setTextPress(QString)));
    emit clearKeyK(2);
    emit sendString(ui->lineEdit_2->text());
    emit setOncePoint(1);
}

void Measurement_config::setTextPress(QString str) // функция приема сигнала от клавиатуры давления и обновления значения
{
    ui->lineEdit_2->setText(str);
    double x= ui->lineEdit_2->text().toDouble()*101.3/760;
    ui->lineEdit_3->setText(QString::number(x,10,3));
}

void Measurement_config::on_pushButton_5_clicked() // кнопка клавиатуры поправочного коэффициента
{
    keyboard *key = new keyboard(this);
    key->setModal(true);
    key->showFullScreen();
    connect(this,SIGNAL(clearKeyK(int)),key,SLOT(clearKeyK(int)));
    connect(this,SIGNAL(setOncePoint(int)),key,SLOT(setOncePoint(int)));
    connect(this,SIGNAL(sendString(QString)),key,SLOT(setKeyString(QString)));
    connect(key,SIGNAL(setText(QString)),this,SLOT(setTextCor_fact(QString)));
    emit clearKeyK(2);
    emit sendString(ui->lineEdit_4->text());
    emit setOncePoint(1);

}
void Measurement_config::setTextCor_fact(QString str) // функция приема сигнала от клавиатуры давления и обновления значения
{
    ui->lineEdit_4->setText(str);
}

void Measurement_config::on_radioButton_clicked() // радио кнопка порога - нет
{
    ui->lineEdit_5->setEnabled(false);
    ui->lineEdit_6->setEnabled(false);
    ui->pushButton_6->setEnabled(false);
    ui->pushButton_7->setEnabled(false);
}

void Measurement_config::on_radioButton_2_clicked() // радио кнопка порога - по дозе
{
     ui->lineEdit_6->setEnabled(false);
     ui->pushButton_7->setEnabled(false);
     ui->lineEdit_5->setEnabled(true);
     ui->pushButton_6->setEnabled(true);
}

void Measurement_config::on_radioButton_3_clicked() // радио кнопка порога - по времени
{
    ui->lineEdit_5->setEnabled(false);
    ui->pushButton_6->setEnabled(false);
    ui->lineEdit_6->setEnabled(true);
    ui->pushButton_7->setEnabled(true);
}

void Measurement_config::on_pushButton_9_clicked() // кнопка Ок
{
    if(QSqlDatabase::contains("myDB"))                      // обновление БД по записям на форме
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("/home/pi/chambers.db");
        if(!db.open())
        {
            qDebug()<<"db was not opened";
        }
        QSqlQuery query(db);
        QString update = "UPDATE config SET voltage='%1', temperature='%2', pressure='%3', correction_factor='%4', "
                         "threshold_dose='%5', threshold_time='%6';";
        QString update1 = update.arg(ui->label_3->text())
                                .arg(ui->lineEdit->text())
                                .arg(ui->lineEdit_2->text())
                                .arg(ui->lineEdit_4->text())
                                .arg(ui->lineEdit_5->text())
                                .arg(ui->lineEdit_6->text());
        if(!query.exec(update1))
        {
            qDebug()<<"unable execute query update";
        }
        db.close();
    }

    if(ui->radioButton->isChecked())
    {
        if(QSqlDatabase::contains("myDB"))
        {
            QSqlDatabase db = QSqlDatabase::database("myDB");
            db.setDatabaseName("/home/pi/chambers.db");
            if(!db.open())
            {
                qDebug()<<"db was not opened";
            }
            QSqlQuery query(db);
            QString update = "UPDATE config SET threshold='%1'";
            QString update1 = update.arg("no");
            if(!query.exec(update1))
            {
                qDebug()<<"unable execute query update";
            }
            db.close();
        }
    }
    if (ui->radioButton_2->isChecked())
    {
        if(QSqlDatabase::contains("myDB"))
        {
            QSqlDatabase db = QSqlDatabase::database("myDB");
            db.setDatabaseName("/home/pi/chambers.db");
            if(!db.open())
            {
                qDebug()<<"db was not opened";
            }
            QSqlQuery query(db);
            QString update = "UPDATE config SET threshold='%1'";
            QString update1 = update.arg("dose");
            if(!query.exec(update1))
            {
                qDebug()<<"unable execute query update";
            }
            db.close();
        }
    }
    if (ui->radioButton_3->isChecked())
    {
        if(QSqlDatabase::contains("myDB"))
        {
            QSqlDatabase db = QSqlDatabase::database("myDB");
            db.setDatabaseName("/home/pi/chambers.db");
            if(!db.open())
            {
                qDebug()<<"db was not opened";
            }
            QSqlQuery query(db);
            QString update = "UPDATE config SET threshold='%1'";
            QString update1 = update.arg("time");
            if(!query.exec(update1))
            {
                qDebug()<<"unable execute query update";
            }
            db.close();
        }
    }
    emit updateMes();
    if (F==1)
    {
        emit okMesConf();
    }
    QString voltage = ui->label_3->text();
    int intVolt=voltage.toInt();
    QByteArray byteVolt;
    voltHex voltHex;

    byteVolt=voltHex.intVolt(intVolt); // - через отдельный класс переводим напряжение в цифрах в hex код для посылки в СОМ порт (7х8х)
    //qDebug()<<QString::number(byteVolt[0],16)<<QString::number(byteVolt[1],16);

    emit bytesVolt(byteVolt);
    emit bytesVolt(byteVolt);
    emit bytesVolt1(byteVolt);
    emit bytesVolt1(byteVolt);
    close();
}

void Measurement_config::firstTimeFlag()
{
    F=1;
}

void Measurement_config::on_pushButton_6_clicked() // кнопка клавиатуры порога по дозе
{
    keyboard *key = new keyboard(this);
    key->setModal(true);
    key->showFullScreen();

    connect(this,SIGNAL(clearKeyK(int)),key,SLOT(clearKeyK(int)));
    connect(this,SIGNAL(setOncePoint(int)),key,SLOT(setOncePoint(int)));
    connect(this,SIGNAL(sendString(QString)),key,SLOT(setKeyString(QString)));
    connect(key,SIGNAL(setText(QString)),this,SLOT(setTextThreashold_dose(QString)));
    emit clearKeyK(2);
    emit sendString(ui->lineEdit_5->text());
    emit setOncePoint(1);
}
void Measurement_config::setTextThreashold_dose(QString str)
{
    ui->lineEdit_5->setText(str);
}

void Measurement_config::on_pushButton_7_clicked() // кнопка клавиатуры порога по времени
{
    keyboard *key = new keyboard(this);
    key->setModal(true);
    key->showFullScreen();

    connect(this,SIGNAL(clearKeyK(int)),key,SLOT(clearKeyK(int)));
    //connect(this,SIGNAL(setOncePoint(int)),key,SLOT(setOncePoint(int)));
    connect(this,SIGNAL(sendString(QString)),key,SLOT(setKeyString(QString)));
    connect(key,SIGNAL(setText(QString)),this,SLOT(setTextThreashold_time(QString)));
    emit clearKeyK(3);
    emit sendString(ui->lineEdit_6->text());
    //emit setOncePoint(1);
}
void Measurement_config::setTextThreashold_time(QString str)
{
    ui->lineEdit_6->setText(str);
}
