#include "chambers.h"
#include "ui_chambers.h"

Chambers::Chambers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Chambers)
{
    ui->setupUi(this);
    F=0; // флаг окну об открытии впервые после теста или в меню. (1-после теста 0 - в меню (по умолчанию в меню))

    QSqlDatabase::addDatabase("QSQLITE", "myDB");    // драйвер базы данных

    if (QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("/home/pi/chambers.db");   // подключение к базе данных
        if (!db.open())
        {
            qDebug()<<"db was not open";
        }
        QSqlQuery query(db); // объект запросов к базе данных

        if(!query.exec("SELECT * FROM chambers"))   // выбрать из всех столбцов таблицы chambers
        {
            qDebug()<<"Unable to execute query";
        }
        QSqlRecord rec = query.record();
        QStringList    num;
        while (query.next())
        {
            num.append(query.value(rec.indexOf("number")).toString()+" "+query.value(rec.indexOf("name")).toString()); //добавление названий в QStringList
            }
         //qDebug() << db.tables();
           // QSqlDatabase::removeDatabase("chambers");
        ui->comboBox->addItems(num); // названия в comboBox

        QSqlQuery query1(db);

        if(!query1.exec("SELECT * FROM chambers;"))
        {
            qDebug()<<"Unable to execute queryYYY";
            qDebug()<<query1.lastError().text();
        }
        query1.first();                                                                       // отображение информации о первой камере в textEdit
        ui->textEdit->setText("название: "+ query1.value("name").toString() +"\n"+
                              "тип: "+query1.value("type").toString() +"\n"+
                              "объем: "+query1.value("volume").toString() +"\n"+
                              "чувствительность: "+query1.value("sensibility").toString() +"\n"+
                              "напряжение: "+query1.value("voltage").toString() +"\n"+
                              "Зав. номер: "+query1.value("factoryNumber").toString() +"\n"+
                              "дата изготовления: "+query1.value("factoryDate").toString() +"\n"+
                              "дата последней поверки: "+query1.value("lastVerificationDate").toString() +"\n"+
                              "размерность чувствительности: "+query1.value("dimensionSensibility").toString());
    }

}

Chambers::~Chambers()
{
    delete ui;
}

void Chambers::on_pushButton_clicked()
{
    close();
}

void Chambers::on_pushButton_2_clicked() // кнопка добавить
{
    Add_chamber* newChamber= new Add_chamber(this);
    newChamber->setModal(true);
    newChamber->showFullScreen();
    connect(newChamber,SIGNAL(newCombo()),this,SLOT(newCombo()));
}

void Chambers::on_pushButton_4_clicked() // кнопка удалить
{
    QMessageBox msg;      // вызывается уточняющий MessageBox
    msg.setModal(true);
    msg.setIcon(QMessageBox::Information);
    QString delChamber="Точно хотите удалить камеру: '%1'";
    QString delChamber1 = delChamber.arg(ui->comboBox->currentText());
    msg.setText(delChamber1);
    msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msg.setDefaultButton(QMessageBox::Cancel);
    int ret = msg.exec();
   // Собственно вот этот case и отвечает за обработку событий
    switch (ret) {
    case QMessageBox::Cancel:
         // Сюда пишем обработку события Cancel
        msg.close();
         break;
      case QMessageBox::Ok:                     // кнопка Ок в QMessageBox
         // Сюда пишем обработку события Ok
        if (QSqlDatabase::contains("myDB"))
        {
           QSqlDatabase db = QSqlDatabase::database("myDB");
           db.setDatabaseName("/home/pi/chambers.db");
           if (!db.open())
           {
               qDebug()<<"db was not open";
           }
           QSqlQuery query(db);
           if(!query.exec("SELECT * FROM chambers;"))
           {
               qDebug()<<"Unable execute query";
           }
           QString delNum = ui->comboBox->currentText();
           QString newStr = delNum.left(delNum.indexOf(' ')); // - новая строка до пробела.

           QString del = "DELETE FROM chambers WHERE number='%1'";
           QString del1 = del.arg(newStr);
           if(!query.exec(del1))
           {
               qDebug()<<"Can not delete";
           }

           int delInt=newStr.toInt();

           if(!query.exec("SELECT * FROM chambers;"))
           {
               qDebug()<<"Unable execute query";
           }
           ui->comboBox->clear(); // очистка comboBox при удалении самого последнего элемента
           QStringList strQuery;

           while (query.next())
           {
               strQuery.append(query.value("number").toString()+" "+query.value("name").toString());
           }

           ui->comboBox->addItems(strQuery); // чтобы не очищалось при удалении последнего
           if(!query.exec("SELECT * FROM chambers;"))
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
                  QString newNum1 = newNum.arg(QString::number(valueNumber-1)).arg(QString::number(valueNumber)); //уменьшает номера последующих камер на единицу
                   if(!query1.exec(newNum1))
                   {
                       qDebug()<<"ne ne ne";
                   }
                   ui->comboBox->clear();
                   if (QSqlDatabase::contains("myDB"))
                   {
                       QSqlDatabase db = QSqlDatabase::database("myDB");
                       db.setDatabaseName("/home/pi/chambers.db");
                       if (!db.open())
                       {
                           qDebug()<<"db was not open";
                       }

                       QSqlQuery query(db);
                       if(!query.exec("SELECT * FROM chambers"))
                       {
                           qDebug()<<"Unable to execute query";
                       }
                       QSqlRecord rec = query.record();

                       QStringList    num;
                       while (query.next())
                       {
                           num.append(query.value(rec.indexOf("number")).toString()+" "+query.value(rec.indexOf("name")).toString());
                           }
                        qDebug() << db.tables();
                          // QSqlDatabase::removeDatabase("chambers");
                       ui->comboBox->addItems(num);
                   }
               }
           }
           QSqlQuery query1(db);

           if(!query1.exec("SELECT * FROM chambers;"))
           {
               qDebug()<<"Unable to execute queryYYY";
               qDebug()<<query1.lastError().text();
           }
           if(!query1.first())
           {
               qDebug()<<"OOO"<<query.lastError().text();
           }
           ui->textEdit->setText("название: "+ query1.value("name").toString() +"\n"+
                                 "тип: "+query1.value("type").toString() +"\n"+
                                 "объем: "+query1.value("volume").toString() +"\n"+
                                 "чувствительность: "+query1.value("sensibility").toString() +"\n"+
                                 "напряжение: "+query1.value("voltage").toString() +"\n"+
                                 "Зав. номер: "+query1.value("factoryNumber").toString() +"\n"+
                                 "дата изготовления: "+query1.value("factoryDate").toString() +"\n"+
                                 "дата последней поверки: "+query1.value("lastVerificationDate").toString() +"\n"+
                                 "размерность чувствительности: "+query1.value("dimensionSensibility").toString());
           db.close();
        }
        break;
        default:
         // Сюда пишем обработку события по умолчанию
        break;
    }
}

void Chambers::on_comboBox_activated()
{
   //ui->textEdit->setText(ui->comboBox->currentText());

   QString combo = ui->comboBox->currentText();
   QString combo1 = combo.left(combo.indexOf(' '));
   int comboInt = combo1.toInt();
   if (QSqlDatabase::contains("myDB"))
   {
       QSqlDatabase db1 = QSqlDatabase::database("myDB");
       db1.setDatabaseName("/home/pi/chambers.db");
       if(!db1.open())
       {
           qDebug()<<"db was not open";
       }
       QSqlQuery query1(db1);

       if(!query1.exec("SELECT * FROM chambers;"))
       {
           qDebug()<<"Unable to execute queryYYY";
           qDebug()<<query1.lastError().text();
       }
       while(query1.next())
       {
           int queryNum = query1.value("number").toInt();
           if (comboInt==queryNum)
           {
               ui->textEdit->setText("название: "+ query1.value("name").toString() +"\n"+
                                     "тип: "+query1.value("type").toString() +"\n"+
                                     "объем: "+query1.value("volume").toString() +"\n"+
                                     "чувствительность: "+query1.value("sensibility").toString() +"\n"+
                                     "напряжение: "+query1.value("voltage").toString() +"\n"+
                                     "Зав. номер: "+query1.value("factoryNumber").toString() +"\n"+
                                     "дата изготовления: "+query1.value("factoryDate").toString() +"\n"+
                                     "дата последней поверки: "+query1.value("lastVerificationDate").toString() +"\n"+
                                     "размерность чувствительности: "+query1.value("dimensionSensibility").toString());
           }
       }
   }
}
void Chambers::newCombo() // обновление comboBox при добавлении камеры
{

    if (QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        if(!db.open())
        {
            qDebug()<<"db was not open";
        }
        db.setDatabaseName("/home/pi/chambers.db");
        QStringList strList;
        QSqlQuery query1(db);
        if(!query1.exec("SELECT * FROM chambers"))
        {
            qDebug()<<"unable execute querYYY";
        }
        QSqlRecord rec=query1.record();
        while(query1.next())
        {
            strList.append(query1.value("number").toString()+" "+query1.value("name").toString());
        }        
        ui->comboBox->clear();
        ui->comboBox->addItems(strList);

        if(!query1.exec("SELECT * FROM chambers"))
        {
            qDebug()<<"unable execute queryyyy";
        }
        if(!query1.first())
        {
            qDebug()<<"unable execute queryyyy";
        }
        ui->textEdit->setText("название: "+ query1.value("name").toString() +"\n"+
                              "тип: "+query1.value("type").toString() +"\n"+
                              "объем: "+query1.value("volume").toString() +"\n"+
                              "чувствительность: "+query1.value("sensibility").toString() +"\n"+
                              "напряжение: "+query1.value("voltage").toString() +"\n"+
                              "Зав. номер: "+query1.value("factoryNumber").toString() +"\n"+
                              "дата изготовления: "+query1.value("factoryDate").toString() +"\n"+
                              "дата последней поверки: "+query1.value("lastVerificationDate").toString() +"\n"+
                              "размерность чувствительности: "+query1.value("dimensionSensibility").toString());
    }
}

void Chambers::on_pushButton_3_clicked() // кнопка правка
{
    Add_chamber *editChamber = new Add_chamber();
    editChamber->setModal(true);
    editChamber->showFullScreen();
    connect(this,SIGNAL(constr(QString)),editChamber,SLOT(constr(QString))); // отправка сигнала с номером камеры
    connect(editChamber,SIGNAL(newCombo()),this,SLOT(newCombo()));           // прием сигнала обновления comboBox

    emit constr(ui->comboBox->currentText().left(ui->comboBox->currentText().indexOf(' '))); //передача сигнала строки с номером камеры
}

void Chambers::on_pushButton_5_clicked() //кнопка Ок - выбора камеры и передачи сигнала о номере камеры в базе данных
{
    QMessageBox msg;
    msg.setModal(true);
    msg.setIcon(QMessageBox::Information);
    QString strMsg = "Выбрана камера - '%1'";
    QString strMsg1=strMsg.arg(ui->comboBox->currentText());
    msg.setText(strMsg1);
    msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msg.setDefaultButton(QMessageBox::Cancel);
    int ret=msg.exec();
    switch (ret)
    {
        case QMessageBox::Cancel:
        msg.close();
        break;
        case QMessageBox::Ok:
        QString numChamber;
        numChamber=ui->comboBox->currentText().left(ui->comboBox->currentText().indexOf(' ')); // присваивание переменной строки до пробела (номера камеры)
        emit butOk(numChamber);

        if (QSqlDatabase::contains("myDB"))
        {
            QString numChamber;
            numChamber=ui->comboBox->currentText().left(ui->comboBox->currentText().indexOf(' ')); // присваивание переменной строки до пробела (номера камеры)

            QSqlDatabase db=QSqlDatabase::database("myDB");
            db.setDatabaseName("/home/pi/chambers.db");
            if(!db.open())
            {
                qDebug()<<"db was not open";
            }
            QSqlQuery query1(db);
            QString find = "SELECT * FROM chambers WHERE number='%1'";
            QString find1 = find.arg(numChamber);
            if(!query1.exec(find1))
            {
                qDebug()<<"unable execute query";
            }
            QString name,sensibility,voltage,dimensionSensibility,type,volume;

            if(query1.first())
            {
                name=query1.value("name").toString();
                sensibility=query1.value("sensibility").toString();
                voltage=query1.value("voltage").toString();
                dimensionSensibility=query1.value("dimensionSensibility").toString().left(query1.value("dimensionSensibility").toString().indexOf('/'));
                type=query1.value("type").toString();
                volume=query1.value("volume").toString();
            }
            qDebug() << name<<sensibility <<voltage <<dimensionSensibility;

            QSqlQuery query(db);
            QString create= "CREATE TABLE IF NOT EXISTS config ("                                            //создаем новую таблицу из одной строки почти (результаты калибровочного теста в файле calibration)
                                                    "id INTEGER PRIMARY KEY CHECK (id = 0),"    //со всеми параметрами необходимыми при измерении
                                                    "name TEXT(10),"                            //параметры из таблицы редактируются в формах с помощью команды UPDATE
                                                    "sensibility TEXT(10),"                     //(в таблице всегда одна строка т.к. id=0)
                                                    "voltage TEXT(10),"
                                                    "dimensionSensibility TEXT(10),"
                                                    "temperature TEXT(10),"
                                                    "pressure TEXT(10),"
                                                    "correction_factor TEXT(10),"
                                                    "threshold TEXT(10),"
                                                    "threshold_time TEXT(10),"
                                                    "threshold_dose_dimension TEXT(10),"
                                                    "threshold_dose TEXT(10),"
                                                    "measurement_mode TEXT(10),"
                                                    "measurement_automode_times TEXT(10),"
                                                    "measuring_parameter TEXT(10),"
                                                    "measuring_parameter_dimension TEXT(10),"
                                                    "compensation1 TEXT(10),"
                                                    "compensation2 TEXT(10),"
                                                    "compensation3 TEXT(10),"
                                                    "account_compensation TEXT(10),"
                                                    "add_filter TEXT(10),"
                                                    "standart_deviation_show TEXT(10),"
                                                    "measurement_range TEXT(10),"
                                                    "type TEXT(10),"
                                                    "volume TEXT(10)"
                                                    ")";

            if(!query.exec(create))
            {
                qDebug()<<"Can not create TABLE config"<<query.lastError().text();
            }

            QString insert =
                "INSERT INTO config (id, name,sensibility,voltage, dimensionSensibility, temperature, pressure, "
                "correction_factor,threshold, threshold_time, threshold_dose_dimension, threshold_dose, measurement_mode, measurement_automode_times,"
                "measuring_parameter, measuring_parameter_dimension, compensation1, compensation2, compensation3, account_compensation,"
                "add_filter, standart_deviation_show, measurement_range, type, volume) "
                "VALUES (%1,'%2','%3','%4','%5','%6','%7','%8','%9','%10','%11','%12','%13','%14','%15','%16','%17','%18',"
                "'%19','%20','%21','%22','%23','%24','%25');";
            QString insert1 = insert.arg(0)
                                .arg(name) // name
                                .arg(sensibility) // sensibility
                                .arg(voltage) // voltage
                                .arg(dimensionSensibility) // dimensionSensibility
                                .arg("5") // temperature
                                .arg("6") // pressure
                                .arg("7") // correction_factor
                                .arg("8") // threshold
                                .arg("9") //threshold_time
                                .arg("10") //threshold_dose_dimension
                                .arg("11") //threshold_dose
                                .arg("12") //measurement_mode
                                .arg("13") //measurement_automode_times
                                .arg("14") //measuring_parameter
                                .arg("15") //measuring_parameter_dimension
                                .arg("16") //compensation1
                                .arg("17") //compensation2
                                .arg("18") //compensation3
                                .arg("19") //account_compensation
                                .arg("20") //add_filter
                                .arg("21") //standart_deviation_show
                                .arg("22") //measurement_range
                                .arg("23") //type
                                .arg("24");//volume

            if (!query.exec(insert1))
            {
                qDebug() << "Unable to do insert opeation";
                qDebug()<<query.lastError().text();
            }
            QString update = "UPDATE config SET name='%1', sensibility='%2', voltage='%3', dimensionSensibility='%4',"
                             "type='%5', volume='%6';";
            QString update1 = update.arg(name)
                                    .arg(sensibility)
                                    .arg(voltage)
                                    .arg(dimensionSensibility)
                                    .arg(type)
                                    .arg(volume);

            if(!query.exec(update1))
            {
                qDebug()<<"unable execute query update";
                qDebug()<<query.lastError().text();
            }
        }

        if(QSqlDatabase::contains("myDB"))
        {
            QSqlDatabase db = QSqlDatabase::database("myDB");
            db.setDatabaseName("/home/pi/chambers.db");
            db.open();
            QSqlQuery query(db);
            QString update = "UPDATE config SET threshold_dose_dimension='%1'"; // флаг о выполнении компенсации в БД (в свободную threshold_dose_dimension)
            QString update1 = update.arg("0");
            if(!query.exec(update1))
            {
                qDebug()<<"unable execute query threshold_dose_dimension";
            }
            db.close();
        }


        emit updateData();

        //Measuring_parameters *mes = new Measuring_parameters(this);
        //connect(this,SIGNAL(updateMesuaring_parameters()),mes,SLOT(updateMesuaring_parameters()));
        //emit updateMesuaring_parameters();
        close();
        qDebug()<<F;
        if(F==1)
        {
            emit okChambers();
        }
        break;        
    }    
}
void Chambers::firstTimeFlag()
{
    F=1;
}
