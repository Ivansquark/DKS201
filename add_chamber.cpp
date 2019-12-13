#include "add_chamber.h"
#include "ui_add_chamber.h"

Add_chamber::Add_chamber(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Add_chamber)
{
    ui->setupUi(this);
    QSqlDatabase::addDatabase("QSQLITE", "myDB");
    ui->comboBox->addItem("Сферическая");
    ui->comboBox->addItem("Наперстковая");
    ui->comboBox_2->addItem("мГр/нКл");
    ui->comboBox_2->addItem("мЗв/нКл");
    ui->comboBox_2->addItem("мР/нКл");

}

Add_chamber::~Add_chamber()
{
    delete ui;
}

void Add_chamber::on_pushButton_clicked()
{
    close();
    //destroy();
}

void Add_chamber::on_pushButton_9_clicked() // Добавить (Исправить) button push (pushButton_9)
{
    if (ui->pushButton_9->text()=="Добавить")
    {
        if (QSqlDatabase::contains("myDB"))
        {
            QSqlDatabase db = QSqlDatabase::database("myDB");
            db.setDatabaseName("/home/pi/chambers.db");
            if (!db.open())
            {
                qDebug()<<"db was not open";
            }
            QSqlQuery query(db);
            QString create = "CREATE TABLE IF NOT EXISTS chambers ("
                                        "number TEXT(2),"
                                        "name TEXT(10),"
                                        "type TEXT(10),"
                                        "volume TEXT(10),"
                                        "sensibility TEXT(10),"
                                        "voltage TEXT(10),"
                                        "factoryNumber TEXT(10),"
                                        "factoryDate TEXT(10),"
                                        "lastVerificationDate TEXT(10),"
                                        "dimensionSensibility TEXT(10)"
                                        ");";

            if (!query.exec(create))
            {
                qDebug() << "Unable to create a table";
            }

            if (!query.exec("SELECT * FROM chambers"))
            {
                qDebug()<<"Unable execute query";
            }
            QSqlRecord rec;
            int num=0;
            if (query.last()!=0)
            {
                num=query.value("number").toInt();
                //qDebug()<<"query not null  "<<num;
            }
            num+=1;
            QString strNum;
            strNum=QString::number(num);

            QString insert =
                "INSERT INTO chambers (number, name,type,volume,sensibility, voltage, factoryNumber, factoryDate, lastVerificationDate,dimensionSensibility) "
                "VALUES ('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10');";
            QString insert1 = insert.arg(strNum)
                                .arg(ui->lineEdit->text()) // name
                                .arg(ui->comboBox->currentText()) // type
                                .arg(ui->lineEdit_2->text()) // volume
                                .arg(ui->lineEdit_3->text()) // sensibility
                                .arg(ui->lineEdit_4->text()) // voltage
                                .arg(ui->lineEdit_5->text()) // factoryNumber
                                .arg(ui->lineEdit_6->text()) // factoryDate
                                .arg(ui->lineEdit_7->text()) // lastVerificationDate
                                .arg(ui->comboBox_2->currentText()); //dimensionSensibility

            if (!query.exec(insert1))
            {
                qDebug() << "Unable to do insert opeation";
            }
        }
        emit newCombo();
        close();
        destroy();
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
            query.prepare("UPDATE chambers SET name=:name, type=:type, volume=:volume,sensibility=:sensibility,"
                          "voltage=:voltage,factoryNumber=:factoryNumber,factoryDate=:factoryDate,"
                          "lastVerificationDate=:lastVerificationDate,dimensionSensibility=:dimensionSensibility"
                          " WHERE number=:number");
            query.bindValue(":name",ui->lineEdit->text());
            query.bindValue(":type",ui->comboBox->currentText());
            query.bindValue(":volume",ui->lineEdit_2->text());
            query.bindValue(":sensibility",ui->lineEdit_3->text());
            query.bindValue(":voltage",ui->lineEdit_4->text());
            query.bindValue(":factoryNumber",ui->lineEdit_5->text());
            query.bindValue(":factoryDate",ui->lineEdit_6->text());
            query.bindValue(":lastVerificationDate",ui->lineEdit_7->text());
            query.bindValue(":dimensionSensibility",ui->comboBox_2->currentText());
            query.bindValue(":number",ui->label_11->text());
            if(!query.exec())
            {
                qDebug()<<"Was not updated";
            }
        }
        emit newCombo();
        close();
        destroy();
    }

}

void Add_chamber::constr(QString num) // передача сигнала о выставлении значений в форме по базе данных
{
    ui->pushButton_9->setText("Исправить");
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
        if(!query.exec("SELECT * FROM chambers"))
        {
            qDebug()<<"unable execute query!!!";
        }
        while(query.next())
        {
            QString numStr = query.value("number").toString();
            if(numInt==numStr.toInt())
            {
                ui->lineEdit->setText(query.value("name").toString());
                ui->comboBox->setCurrentText(query.value("type").toString());
                ui->lineEdit_2->setText(query.value("volume").toString());
                ui->lineEdit_3->setText(query.value("sensibility").toString());
                ui->lineEdit_4->setText(query.value("voltage").toString());
                ui->lineEdit_5->setText(query.value("factoryNumber").toString());
                ui->lineEdit_6->setText(query.value("factoryDate").toString());
                ui->lineEdit_7->setText(query.value("lastVerificationDate").toString());
                ui->comboBox_2->setCurrentText(query.value("dimensionSensibility").toString());

                ui->label_11->setText(query.value("number").toString());
            }
        }
    }
}

void Add_chamber::on_pushButton_2_clicked() // клавиатура имени
{
    keyboard *key = new keyboard(this);
    connect(this,SIGNAL(clearKeyK(int)),key,SLOT(clearKeyK(int)));
    connect(this,SIGNAL(setKeyString(QString)),key,SLOT(setKeyString(QString)));
    connect(key,SIGNAL(setText(QString)),this,SLOT(setTextLine_1(QString)));

    key->setModal(true);
    key->showFullScreen();
    //emit clearKeyK(1);
    emit setKeyString(ui->lineEdit->text());
}
void Add_chamber::setTextLine_1(QString str)
{
    ui->lineEdit->setText(str);
}

void Add_chamber::on_pushButton_3_clicked() // клавиатура объема
{
    keyboard *key = new keyboard(this);
    connect(this,SIGNAL(clearKeyK(int)),key,SLOT(clearKeyK(int)));
    connect(this,SIGNAL(setKeyString(QString)),key,SLOT(setKeyString(QString)));
    connect(this,SIGNAL(setOncePoint(int)),key,SLOT(setOncePoint(int)));
    connect(key,SIGNAL(setText(QString)),this,SLOT(setTextLine_2(QString)));

    key->setModal(true);
    key->showFullScreen();
    emit clearKeyK(2);          // - сигнал кнопкам по выводу только цифр
    emit setKeyString(ui->lineEdit_2->text());
    emit setOncePoint(1); // - точка только один раз
}
void Add_chamber::setTextLine_2(QString str)
{
    ui->lineEdit_2->setText(str);
}

void Add_chamber::on_pushButton_4_clicked() // клавиатура чувствительности
{
    keyboard *key = new keyboard(this);
    connect(this,SIGNAL(clearKeyK(int)),key,SLOT(clearKeyK(int)));
    connect(this,SIGNAL(setOncePoint(int)),key,SLOT(setOncePoint(int)));
    connect(this,SIGNAL(setKeyString(QString)),key,SLOT(setKeyString(QString)));
    connect(key,SIGNAL(setText(QString)),this,SLOT(setTextLine_3(QString)));

    key->setModal(true);
    key->showFullScreen();
    emit clearKeyK(2);                            // - сигнал кнопкам по выводу только цифр
    emit setKeyString(ui->lineEdit_3->text());
    emit setOncePoint(1);                       // - точка только один раз
}
void Add_chamber::setTextLine_3(QString str)
{
    ui->lineEdit_3->setText(str);
}

void Add_chamber::on_pushButton_5_clicked() // клавиатура напряжения
{
    keyboard *key = new keyboard(this);
    connect(this,SIGNAL(clearKeyK(int)),key,SLOT(clearKeyK(int)));
    connect(this,SIGNAL(setOncePoint(int)),key,SLOT(setOncePoint(int)));
    connect(this,SIGNAL(setKeyString(QString)),key,SLOT(setKeyString(QString)));
    connect(key,SIGNAL(setText(QString)),this,SLOT(setTextLine_4(QString)));

    key->setModal(true);
    key->showFullScreen();
    emit clearKeyK(4);                            // - сигнал кнопкам по выводу напряжения (знак минус и цифры)
    emit setKeyString(ui->lineEdit_4->text());
    emit setOncePoint(2);                       // - минус только один первый раз
}
void Add_chamber::setTextLine_4(QString str)
{
    ui->lineEdit_4->setText(str);
}

void Add_chamber::on_pushButton_6_clicked() // клавиатура заводского номера
{
    keyboard *key = new keyboard(this);
    connect(this,SIGNAL(clearKeyK(int)),key,SLOT(clearKeyK(int)));
    //connect(this,SIGNAL(setOncePoint(int)),key,SLOT(setOncePoint(int)));
    connect(this,SIGNAL(setKeyString(QString)),key,SLOT(setKeyString(QString)));
    connect(key,SIGNAL(setText(QString)),this,SLOT(setTextLine_5(QString)));

    key->setModal(true);
    key->showFullScreen();
    emit clearKeyK(3);                            // - сигнал кнопкам по выводу напряжения (только цифры)
    emit setKeyString(ui->lineEdit_5->text());
    //emit setOncePoint(2);                       // - минус только один первый раз
}
void Add_chamber::setTextLine_5(QString str)
{
    ui->lineEdit_5->setText(str);
}

void Add_chamber::on_pushButton_7_clicked() // клавиатура даты изготовления
{
    keyboard *key = new keyboard(this);
    connect(this,SIGNAL(clearKeyK(int)),key,SLOT(clearKeyK(int)));
    //connect(this,SIGNAL(setOncePoint(int)),key,SLOT(setOncePoint(int)));
    connect(this,SIGNAL(setKeyString(QString)),key,SLOT(setKeyString(QString)));
    connect(key,SIGNAL(setText(QString)),this,SLOT(setTextLine_6(QString)));

    key->setModal(true);
    key->showFullScreen();
    emit clearKeyK(2);                            // - сигнал кнопкам по выводу напряжения (цифры и точки)
    emit setKeyString(ui->lineEdit_6->text());

}
void Add_chamber::setTextLine_6(QString str)
{
    ui->lineEdit_6->setText(str);
}

void Add_chamber::on_pushButton_8_clicked() // клавиатура даты последней поверки
{
    keyboard *key = new keyboard(this);
    connect(this,SIGNAL(clearKeyK(int)),key,SLOT(clearKeyK(int)));

    connect(this,SIGNAL(setKeyString(QString)),key,SLOT(setKeyString(QString)));
    connect(key,SIGNAL(setText(QString)),this,SLOT(setTextLine_7(QString)));

    key->setModal(true);
    key->showFullScreen();
    emit clearKeyK(2);                            // - сигнал кнопкам по выводу напряжения (цифры и точки)
    emit setKeyString(ui->lineEdit_7->text());

}
void Add_chamber::setTextLine_7(QString str)
{
    ui->lineEdit_7->setText(str);
}
