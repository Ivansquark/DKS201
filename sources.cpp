#include "sources.h"
#include "ui_sources.h"

Sources::Sources(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Sources)
{
    ui->setupUi(this);
}

Sources::~Sources()
{
    delete ui;
}

void Sources::init()
{
    QSqlDatabase::addDatabase("QSQLITE","myDB");
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
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
    }
    else if(strList[0]==strList[1])
    {
        //ui->pushButton_3->setText("Правка");
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
    }
    else
    {
        //ui->pushButton_3->setText("Правка");
        ui->pushButton_3->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
    }
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db=QSqlDatabase::database("myDB");
        db.setDatabaseName("/home/pi/chambers.db");
        if (!db.open())
        {
            qDebug()<<"db was not open";
        }
        QSqlQuery query(db);
        QString strQuery = "SELECT *FROM sources";
        if(!query.exec(strQuery))
        {
            qDebug()<<"unable SELECT *FROM sources";
        }
        while (query.next())
        {
            ui->comboBox->addItem(query.value("number").toString()+" "+query.value("sourceName").toString());
        }
        query.first();                                                                       // отображение информации о первой камере в textEdit

        QDate dateVerificationCSd=QDate::fromString(query.value("sourceDateVerification").toString(),"dd.MM.yyyy");
        QDate dateFabricationCSd=QDate::fromString(query.value("sourceDateManufactory").toString(),"dd.MM.yyyy");
        half_lifeCS=query.value("half_life").toDouble();
        doseRateVerificationCS=query.value("doseRateVerification").toDouble();
        activityFabricationCS=query.value("activityManufactory").toDouble();
        doseRateCurrentCS=doseRateVerificationCS*pow(2,-1*(QDate::currentDate().toJulianDay()-dateVerificationCSd.toJulianDay())/(365*half_lifeCS));
        activityCurrentCS=activityFabricationCS*pow(2,-1*(QDate::currentDate().toJulianDay()-dateFabricationCSd.toJulianDay())/(365*half_lifeCS));        
        doseRateDimension = query.value("doseRateDimension").toString();

        QString strDimension = doseRateDimension.remove(2,doseRateDimension.size());

        ui->textEdit->setText("Источник: "+ query.value("sourceName").toString() +"\n"+
                              "Заводской номер: "+query.value("sorceNumber").toString() +"\n"+
                              "Дата изготовления: "+query.value("sourceDateManufactory").toString() +"\n"+
                              "Изотоп: "+query.value("isotope").toString() +"\n"+
                              "Период полураспада, лет: "+query.value("half_life").toString() +"\n"+
                              "Начальная активность: "+query.value("activityManufactory").toString()+" "+"МБк"+"\n"+
                              "Дата последней поверки: "+query.value("sourceDateVerification").toString() +"\n"+
                              "Мощность дозы при поверке: "+query.value("doseRateVerification").toString() +" "+
                              query.value("doseRateDimension").toString()+"\n"+
                              "Активность на настоящее время"+" "+QString::number(activityCurrentCS,'f',3)+" "+"МБк"+"\n"
                              "Мощность дозы на настоящее время"+" "+QString::number(doseRateCurrentCS,'f',3)+" "+query.value("doseRateDimension").toString());
    }
}

void Sources::on_pushButton_clicked()
{
    close();
}

void Sources::on_pushButton_2_clicked() // кнопка добавить
{
    Add_source *add =new Add_source(this);
    connect(add,SIGNAL(newSourceList()),this,SLOT(newSourceList()));
    add->setModal(true);

    add->init1();
    add->showFullScreen();
}

void Sources::on_pushButton_3_clicked()  // кнопка исправить
{
    Add_source *add =new Add_source(this);
    connect(add,SIGNAL(newSourceList()),this,SLOT(newSourceList()));
    add->setModal(true);

    add->init2();
    add->showFullScreen();
    connect(this,SIGNAL(constr(QString)),add,SLOT(constr(QString))); // отправка сигнала с номером камеры
    connect(add,SIGNAL(newSourceList()),this,SLOT(newSourceList()));           // прием сигнала обновления comboBox

    emit constr(ui->comboBox->currentText().left(ui->comboBox->currentText().indexOf(' '))); //передача сигнала строки с номером камеры
}


void Sources::newSourceList()
{
    ui->comboBox->clear();
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db=QSqlDatabase::database("myDB");
        db.setDatabaseName("/home/pi/chambers.db");
        if (!db.open())
        {
            qDebug()<<"db was not open";
        }
        QSqlQuery query(db);
        QString strQuery = "SELECT *FROM sources";
        if(!query.exec(strQuery))
        {
            qDebug()<<"unable SELECT *FROM sources";
        }
        while (query.next())
        {
            ui->comboBox->addItem(query.value("number").toString()+" "+query.value("sourceName").toString());
        }
    }
}

void Sources::on_pushButton_5_clicked() // кнопка ОК
{
    QMessageBox msg;
    msg.setModal(true);
    msg.setIcon(QMessageBox::Information);
    QString strMsg = "Выбран источник -%1";
    QString strMsg1 = strMsg.arg(ui->comboBox->currentText());
    msg.setText(strMsg1);
    msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msg.setDefaultButton(QMessageBox::Cancel);
    int ret = msg.exec();
    switch (ret)
    {
        case QMessageBox::Cancel:
            msg.close();
        break;

        case QMessageBox::Ok:
            QString str = ui->comboBox->currentText().at(0);
            emit setCS(str);
            msg.close();
        break;
    }
    close();
}

void Sources::on_comboBox_activated()
{
    QString combo = ui->comboBox->currentText();
    QString combo1 = combo.left(combo.indexOf(' '));
    int comboInt = combo1.toInt();

    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db=QSqlDatabase::database("myDB");
        db.setDatabaseName("/home/pi/chambers.db");
        if (!db.open())
        {
            qDebug()<<"db was not open";
        }
        QSqlQuery query(db);
        QString strQuery = "SELECT *FROM sources";
        if(!query.exec(strQuery))
        {
            qDebug()<<"unable SELECT *FROM sources";
        }
        while (query.next())
        {
            int queryNum = query.value("number").toInt();
            if(queryNum==comboInt)
            {
                QDate dateVerificationCSd=QDate::fromString(query.value("sourceDateVerification").toString(),"dd.MM.yyyy");
                QDate dateFabricationCSd=QDate::fromString(query.value("sourceDateManufactory").toString(),"dd.MM.yyyy");
                half_lifeCS=query.value("half_life").toDouble();
                doseRateVerificationCS=query.value("doseRateVerification").toDouble();
                activityFabricationCS=query.value("activityManufactory").toDouble();
                doseRateCurrentCS=doseRateVerificationCS*pow(2,-1*(QDate::currentDate().toJulianDay()-dateVerificationCSd.toJulianDay())/(365*half_lifeCS));
                activityCurrentCS=activityFabricationCS*pow(2,-1*(QDate::currentDate().toJulianDay()-dateFabricationCSd.toJulianDay())/(365*half_lifeCS));
                doseRateDimension = query.value("doseRateDimension").toString();
                QString strDimension = doseRateDimension.remove(2,doseRateDimension.size());

                ui->textEdit->setText("Источник: "+ query.value("sourceName").toString() +"\n"+
                                      "Заводской номер: "+query.value("sorceNumber").toString() +"\n"+
                                      "Дата изготовления: "+query.value("sourceDateManufactory").toString() +"\n"+
                                      "Изотоп: "+query.value("isotope").toString() +"\n"+
                                      "Период полураспада, лет: "+query.value("half_life").toString() +"\n"+
                                      "Начальная активность: "+query.value("activityManufactory").toString()+" "+"МБк"+"\n"+
                                      "Дата последней поверки: "+query.value("sourceDateVerification").toString() +"\n"+
                                      "Мощность дозы при поверке: "+query.value("doseRateVerification").toString() +" "+
                                      query.value("doseRateDimension").toString()+"\n"+
                                      "Активность на настоящее время"+" "+QString::number(activityCurrentCS,'f',3)+" "+"МБк"+"\n"
                                      "Мощность дозы на настоящее время"+" "+QString::number(doseRateCurrentCS,'f',3)+" "+query.value("doseRateDimension").toString());
            }
        }
    }
}

void Sources::on_pushButton_4_clicked() // кнопка удалить
{
    QMessageBox msg;
    msg.setModal(true);
    msg.setIcon(QMessageBox::Information);
    msg.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
    msg.setDefaultButton(QMessageBox::Cancel);
    QString strCS = "Точно хотите удалить источник '%1'";
    QString strCS1 = strCS.arg(ui->comboBox->currentText());
    msg.setText(strCS1);
    int ret = msg.exec();
    switch (ret)
    {
        case QMessageBox::Cancel:
        msg.close();break;
    case QMessageBox::Ok:
        if(QSqlDatabase::contains("myDB"))
        {
            QSqlDatabase db = QSqlDatabase::database("myDB");
            db.setDatabaseName("/home/pi/chambers.db");
            if(!db.open())
            {qDebug()<<"db was not opened";}
            QSqlQuery query(db);
            if(!query.exec("SELECT * FROM sources;"))
            {
                qDebug()<<"Unable execute query";
            }
            QString delNum = ui->comboBox->currentText();
            QString newStr = delNum.left(delNum.indexOf(' ')); // - новая строка до пробела.

            QString del = "DELETE FROM sources WHERE number='%1'";
            QString del1 = del.arg(newStr);
            if(!query.exec(del1))
            {
                qDebug()<<"Can not delete";
            }

            int delInt=newStr.toInt();
            if(!query.exec("SELECT * FROM sources;"))
            {
                qDebug()<<"Unable execute query";
            }
            ui->comboBox->clear(); // очистка comboBox при удалении самого последнего элемента
            QStringList strQuery;
            while (query.next())
            {
                strQuery.append(query.value("number").toString()+" "+query.value("sourceName").toString());
            }
            ui->comboBox->addItems(strQuery); // чтобы не очищалось при удалении последнего
            if(!query.exec("SELECT * FROM sources;"))
            {
                qDebug()<<"Unable execute query";
            }

            while (query.next())
            {
                qDebug()<<query.value("number").toInt();
                int valueNumber = query.value("number").toInt();
                if (valueNumber>delInt)
                {
                   QSqlQuery query1(db);
                   QString newNum = "UPDATE chambers SET number='%1' WHERE number='%2'";
                   QString newNum1 = newNum.arg(QString::number(valueNumber-1)).arg(QString::number(valueNumber)); //уменьшает номера последующих источников на единицу
                    if(!query1.exec(newNum1))
                    {
                        qDebug()<<"ne ne ne";
                    }
                    ui->comboBox->clear();
                    if (QSqlDatabase::contains("myDB"))
                    {
                        QSqlDatabase db = QSqlDatabase::database("myDB");
                        db.setDatabaseName("chambers.db");
                        if (!db.open())
                        {
                            qDebug()<<"db was not open";
                        }

                        QSqlQuery query(db);
                        if(!query.exec("SELECT * FROM sources"))
                        {
                            qDebug()<<"Unable to execute query";
                        }

                        QStringList num;
                        while (query.next())
                        {
                            num.append(query.value("number").toString()+" "+query.value("sourceName").toString());
                        }
                           // QSqlDatabase::removeDatabase("chambers");
                        ui->comboBox->addItems(num);
                    }
                }
            }
            QString combo = ui->comboBox->currentText();
            QString combo1 = combo.left(combo.indexOf(' '));
            int comboInt = combo1.toInt();
            QSqlQuery query1(db);
            if(!query1.exec("SELECT *FROM sources"))
            {
                qDebug()<<"unable SELECT *FROM sources";
            }
            while (query.next())
            {
                int queryNum = query.value("number").toInt();
                if(queryNum==comboInt)
                {
                    QDate dateVerificationCSd=QDate::fromString(query.value("sourceDateVerification").toString(),"dd.MM.yyyy");
                    QDate dateFabricationCSd=QDate::fromString(query.value("sourceDateManufactory").toString(),"dd.MM.yyyy");
                    half_lifeCS=query.value("half_life").toDouble();
                    doseRateVerificationCS=query.value("doseRateVerification").toDouble();
                    activityFabricationCS=query.value("activityManufactory").toDouble();
                    doseRateCurrentCS=doseRateVerificationCS*pow(2,-1*(QDate::currentDate().toJulianDay()-dateVerificationCSd.toJulianDay())/(365*half_lifeCS));
                    activityCurrentCS=activityFabricationCS*pow(2,-1*(QDate::currentDate().toJulianDay()-dateFabricationCSd.toJulianDay())/(365*half_lifeCS));
                    doseRateDimension = query.value("doseRateDimension").toString();
                    QString strDimension = doseRateDimension.remove(2,doseRateDimension.size());

                    ui->textEdit->setText("Источник: "+ query.value("sourceName").toString() +"\n"+
                                          "Заводской номер: "+query.value("sorceNumber").toString() +"\n"+
                                          "Дата изготовления: "+query.value("sourceDateManufactory").toString() +"\n"+
                                          "Изотоп: "+query.value("isotope").toString() +"\n"+
                                          "Период полураспада, лет: "+query.value("half_life").toString() +"\n"+
                                          "Начальная активность: "+query.value("activityManufactory").toString()+" "+"МБк"+"\n"+
                                          "Дата последней поверки: "+query.value("sourceDateVerification").toString() +"\n"+
                                          "Мощность дозы при поверке: "+query.value("doseRateVerification").toString() +" "+
                                          query.value("doseRateDimension").toString()+"\n"+
                                          "Активность на настоящее время"+" "+QString::number(activityCurrentCS,'f',3)+" "+"МБк"+"\n"
                                          "Мощность дозы на настоящее время"+" "+QString::number(doseRateCurrentCS,'f',3)+" "+query.value("doseRateDimension").toString());
                }
            }

        }

    }
}
