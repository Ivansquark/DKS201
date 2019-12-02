#include "measuring_parameters.h"
#include "ui_measuring_parameters.h"

Measuring_parameters::Measuring_parameters(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Measuring_parameters)
{
    ui->setupUi(this);

}

Measuring_parameters::~Measuring_parameters()
{
    delete ui;
    qDebug()<<"Destructor Measuring_parameters";
}

void Measuring_parameters::init()
{
    ADC=0;
    nullADC=8388607;

    QFile conf("/home/pi/config.txt");
    QStringList strList;
    if(conf.exists() && conf.open(QIODevice::ReadOnly))
    {
        while(!conf.atEnd())
        {
            strList.append(conf.readLine());
        }
        conf.close();
    }

    R1=strList[1].toDouble();

    QStringList strList1;
    QFile cal("/home/pi/calibration.txt");
    if(cal.exists()&&cal.open(QIODevice::ReadOnly))
    {
        while(!cal.atEnd())
        {
            strList1.append(cal.readLine());
        }
        cal.close();
    }
    R2=strList1[1].toDouble();
    R3=strList1[2].toDouble();

    QSqlDatabase::addDatabase("QSQLITE", "myDB");
    QString dimensionParam;
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("/home/pi/chambers.db");
        db.open();
        QSqlQuery query(db);
        if(!query.exec("SELECT * FROM config"))
        {
            qDebug()<<"unable execute query SELECT";
        }
        if(query.first())
        {
            QString str=query.value("dimensionSensibility").toString();
            qDebug()<<str;
            dimensionParam=str.remove(0,1);
            qDebug()<<dimensionParam;
            ui->comboBox->addItem(dimensionParam+"/с");
            ui->comboBox->addItem(dimensionParam+"/мин");
            ui->comboBox->addItem(dimensionParam+"/ч");
            ui->comboBox->setCurrentText(query.value("measuring_parameter_dimension").toString());
            if(query.value("measuring_parameter").toString()=="1")
            {
                ui->radioButton->setChecked(true);
            }
            if(query.value("measuring_parameter").toString()=="2")
            {
                ui->radioButton_2->setChecked(true);
            }
            if(query.value("measuring_parameter").toString()=="3")
            {
                ui->radioButton_3->setChecked(true);
            }
            if(query.value("measuring_parameter").toString()=="4")
            {
                ui->radioButton_4->setChecked(true);
            }
            if(query.value("measuring_parameter").toString()=="5")
            {
                ui->radioButton_5->setChecked(true);
            }
            if(query.value("measuring_parameter").toString()=="6")
            {
                ui->radioButton_6->setChecked(true);
            }
            if(dimensionParam=="Гр")
            {
                ui->radioButton_4->setText("Мощность поглощенной дозы");
                ui->radioButton_5->setText("Интеграл МПД");
                ui->radioButton_6->setText("Поглощенная доза");
            }
            if(dimensionParam=="Зв")
            {
                ui->radioButton_4->setText("Мощность амбиентного эквивалента дозы");
                ui->radioButton_5->setText("Интеграл МАЭД");
                ui->radioButton_6->setText("Амбиентный эквивалент дозы");
            }
            if(dimensionParam=="Р")
            {
                ui->radioButton_4->setText("Мощность экспозиционной дозы");
                ui->radioButton_5->setText("Интеграл МЭД");
                ui->radioButton_6->setText("Экспозиционная доза");
            }
            if(query.value("account_compensation").toString()=="0")
            {
                ui->checkBox->setCheckState(Qt::Unchecked);
            }
            if(query.value("account_compensation").toString()=="1") ui->checkBox->setCheckState(Qt::Checked);

            if(query.value("threshold_dose_dimension").toString()=="0")
            {
                ui->label_7->setText("Компенсация не проводилась");
                ui->checkBox->setCheckState(Qt::Unchecked);
            }
            if(query.value("threshold_dose_dimension").toString()=="1")
            {
                ui->label_7->setText("Учесть компенсацию?");
                double comp1=query.value("compensation1").toString().toInt()*0.0000003/(R3*1000000000);
                double comp2=query.value("compensation2").toString().toInt()*0.0000003/(R2*1000000);
                double comp3=query.value("compensation3").toString().toInt()*0.0000003/(R1*1000000);
                ui->label_4->setText(QString::number(comp1*1e15)+"пА");
                ui->label_5->setText(QString::number(comp2*1e15)+"пА");
                ui->label_6->setText(QString::number(comp3*1e15)+"пА");
            }
            if(query.value("add_filter").toString()=="0")
            {
                ui->checkBox_2->setCheckState(Qt::Unchecked);
            }
            if(query.value("add_filter").toString()=="1")
            {
                ui->checkBox_2->setCheckState(Qt::Checked);
            }
            if(query.value("standart_deviation_show").toString()=="0")
            {
                ui->checkBox_3->setCheckState(Qt::Unchecked);
            }
            if(query.value("standart_deviation_show").toString()=="1")
            {
                ui->checkBox_3->setCheckState(Qt::Checked);
            }
            ui->comboBox_2->addItem("чувствительный");
            ui->comboBox_2->addItem("средний");
            ui->comboBox_2->addItem("грубый");
            if(query.value("measurement_range").toString()=="1") ui->comboBox_2->setCurrentText("чувствительный");
            if(query.value("measurement_range").toString()=="2") ui->comboBox_2->setCurrentText("средний");
            if(query.value("measurement_range").toString()=="3") ui->comboBox_2->setCurrentText("грубый");
        }
    }
}
void Measuring_parameters::init_CS()
{
    ADC=0;
    nullADC=8388607;
    QFile conf("/home/pi/config.txt");
    QStringList strList;
    if(conf.exists() && conf.open(QIODevice::ReadOnly))
    {
        while(!conf.atEnd())
        {
            strList.append(conf.readLine());
        }
        conf.close();
    }

    R1=strList[1].toDouble();

    QStringList strList1;
    QFile cal("/home/pi/calibration.txt");
    if(cal.exists()&&cal.open(QIODevice::ReadOnly))
    {
        while(!cal.atEnd())
        {
            strList1.append(cal.readLine());
        }
        cal.close();
    }
    R2=strList1[1].toDouble();
    R3=strList1[2].toDouble();

    QSqlDatabase::addDatabase("QSQLITE", "myDB");
    QString dimensionParam;
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("/home/pi/chambers.db");
        db.open();
        QSqlQuery query(db);
        if(!query.exec("SELECT * FROM config"))
        {
            qDebug()<<"unable execute query SELECT";
        }
        if(query.first())
        {
            QString str=query.value("dimensionSensibility").toString();
            qDebug()<<str;
            dimensionParam=str.remove(0,1);
            qDebug()<<dimensionParam;
            ui->comboBox->addItem(dimensionParam+"/с");
            ui->comboBox->addItem(dimensionParam+"/мин");
            ui->comboBox->addItem(dimensionParam+"/ч");
            ui->comboBox->setCurrentText(query.value("measuring_parameter_dimension").toString());
            //ui->comboBox->setEnabled(false);

            ui->radioButton_4->setChecked(true);
            ui->radioButton_3->setEnabled(false);
            ui->radioButton_5->setEnabled(false);
            ui->radioButton_6->setEnabled(false);
            ui->radioButton->setEnabled(false);
            ui->radioButton_2->setEnabled(false);


            if(dimensionParam=="Гр")
            {
                ui->radioButton_4->setText("Мощность поглощенной дозы");
                ui->radioButton_5->setText("Интеграл МПД");
                ui->radioButton_6->setText("Поглощенная доза");
            }
            if(dimensionParam=="Зв")
            {
                ui->radioButton_4->setText("Мощность амбиентного эквивалента дозы");
                ui->radioButton_5->setText("Интеграл МАЭД");
                ui->radioButton_6->setText("Амбиентный эквивалент дозы");
            }
            if(dimensionParam=="Р")
            {
                ui->radioButton_4->setText("Мощность экспозиционной дозы");
                ui->radioButton_5->setText("Интеграл МЭД");
                ui->radioButton_6->setText("Экспозиционная доза");
            }
            if(query.value("account_compensation").toString()=="0")
            {
                ui->checkBox->setCheckState(Qt::Unchecked);
            }
            if(query.value("account_compensation").toString()=="1") ui->checkBox->setCheckState(Qt::Checked);

            if(query.value("threshold_dose_dimension").toString()=="0")
            {
                ui->label_7->setText("Компенсация не проводилась");
                ui->checkBox->setCheckState(Qt::Unchecked);
            }
            if(query.value("threshold_dose_dimension").toString()=="1")
            {
                ui->label_7->setText("Учесть компенсацию?");
                double comp1=query.value("compensation1").toString().toInt()*0.0000003/(R3*1000000000);
                double comp2=query.value("compensation2").toString().toInt()*0.0000003/(R2*1000000);
                double comp3=query.value("compensation3").toString().toInt()*0.0000003/(R1*1000000);
                ui->label_4->setText(QString::number(comp1*1e15)+"пА");
                ui->label_5->setText(QString::number(comp2*1e15)+"пА");
                ui->label_6->setText(QString::number(comp3*1e15)+"пА");
            }
            if(query.value("add_filter").toString()=="0")
            {
                ui->checkBox_2->setCheckState(Qt::Unchecked);
            }
            if(query.value("add_filter").toString()=="1")
            {
                ui->checkBox_2->setCheckState(Qt::Checked);
            }
            if(query.value("standart_deviation_show").toString()=="0")
            {
                ui->checkBox_3->setCheckState(Qt::Unchecked);
            }
            if(query.value("standart_deviation_show").toString()=="1")
            {
                ui->checkBox_3->setCheckState(Qt::Checked);
            }
            ui->comboBox_2->addItem("чувствительный");
            ui->comboBox_2->addItem("средний");
            ui->comboBox_2->addItem("грубый");
            if(query.value("measurement_range").toString()=="1") ui->comboBox_2->setCurrentText("чувствительный");
            if(query.value("measurement_range").toString()=="2") ui->comboBox_2->setCurrentText("средний");
            if(query.value("measurement_range").toString()=="3") ui->comboBox_2->setCurrentText("грубый");
        }
    }
}

void Measuring_parameters::setCStoMesPar() // установка параметров измерения для контрольного источника (нажатие кнопкиОК)
{
    connect(this,SIGNAL(okClick()),this,SLOT(on_pushButton_2_clicked()));

    emit okClick();
    qDebug()<<"emit okClick();";
}


void Measuring_parameters::recieveADCtoMesPar(QString str) //функция приема кода с АЦП для компенсации измерения при подключенной камере
{
    ADC=str.toLong();
    ADC_V=ADC-nullADC;
    ui->label_8->setText(QString::number(ADC_V));
    emit sendADCfromMesPar(QString::number(ADC_V));
}

void Measuring_parameters::on_pushButton_3_clicked() // кнопка отмена
{
    close();
}
void Measuring_parameters::updateMesuaring_parameters()
{
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("/home/pi/chambers.db");
        db.open();
        QSqlQuery query(db);
        QString update = "UPDATE config SET measuring_parameter_dimension='%1'";
        QString update1 = update.arg(ui->comboBox->currentText());
        if(!query.exec(update1))
        {
            qDebug()<<"unable execute query ток";
        }
        db.close();
    }
    close();
    destroy();
}

void Measuring_parameters::on_pushButton_2_clicked() // кнопка ОК
{
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("/home/pi/chambers.db");
        db.open();
        QSqlQuery query(db);
        QString update = "UPDATE config SET measuring_parameter_dimension='%1'";
        QString update1 = update.arg(ui->comboBox->currentText());
        if(!query.exec(update1))
        {
            qDebug()<<"unable execute query ток";
        }
        db.close();
    }

    if(ui->radioButton->isChecked()) //если выбрана радиокнопка ток
    {
        if(QSqlDatabase::contains("myDB"))
        {
            QSqlDatabase db = QSqlDatabase::database("myDB");
            db.setDatabaseName("/home/pi/chambers.db");
            db.open();
            QSqlQuery query(db);
            QString update = "UPDATE config SET measuring_parameter='%1'";
            QString update1 = update.arg("1");
            if(!query.exec(update1))
            {
                qDebug()<<"unable execute query ток";
            }
            db.close();
        }
    }
    if(ui->radioButton_2->isChecked()) //если выбрана радиокнопка  интеграл тока
    {
        if(QSqlDatabase::contains("myDB"))
        {
            QSqlDatabase db = QSqlDatabase::database("myDB");
            db.setDatabaseName("/home/pi/chambers.db");
            db.open();
            QSqlQuery query(db);
            QString update = "UPDATE config SET measuring_parameter='%1'";
            QString update1 = update.arg("2");
            if(!query.exec(update1))
            {
                qDebug()<<"unable execute query интеграл тока";
            }
            db.close();
        }
    }
    if(ui->radioButton_3->isChecked()) //если выбрана радиокнопка заряд
    {
        if(QSqlDatabase::contains("myDB"))
        {
            QSqlDatabase db = QSqlDatabase::database("myDB");
            db.setDatabaseName("/home/pi/chambers.db");
            db.open();
            QSqlQuery query(db);
            QString update = "UPDATE config SET measuring_parameter='%1'";
            QString update1 = update.arg("3");
            if(!query.exec(update1))
            {
                qDebug()<<"unable execute query заряд";
            }
            db.close();
        }
    }
    if(ui->radioButton_4->isChecked()) //если выбрана радиокнопка  Мощность поглощенной дозы
    {
        if(QSqlDatabase::contains("myDB"))
        {
            QSqlDatabase db = QSqlDatabase::database("myDB");
            db.setDatabaseName("/home/pi/chambers.db");
            db.open();
            QSqlQuery query(db);
            QString update = "UPDATE config SET measuring_parameter='%1'";
            QString update1 = update.arg("4");
            if(!query.exec(update1))
            {
                qDebug()<<"unable execute query МПД";
            }
            db.close();
        }
    }
    if(ui->radioButton_5->isChecked()) //если выбрана радиокнопка  интеграл МПД
    {
        if(QSqlDatabase::contains("myDB"))
        {
            QSqlDatabase db = QSqlDatabase::database("myDB");
            db.setDatabaseName("/home/pi/chambers.db");
            db.open();
            QSqlQuery query(db);
            QString update = "UPDATE config SET measuring_parameter='%1'";
            QString update1 = update.arg("5");
            if(!query.exec(update1))
            {
                qDebug()<<"unable execute query интеграл МПД";
            }
            db.close();
        }
    }
    if(ui->radioButton_6->isChecked()) //если выбрана радиокнопка  ПД
    {
        if(QSqlDatabase::contains("myDB"))
        {
            QSqlDatabase db = QSqlDatabase::database("myDB");
            db.setDatabaseName("/home/pi/chambers.db");
            db.open();
            QSqlQuery query(db);
            QString update = "UPDATE config SET measuring_parameter='%1'";
            QString update1 = update.arg("6");
            if(!query.exec(update1))
            {
                qDebug()<<"unable execute query ПД";
            }
            db.close();
        }
    }
    if(ui->checkBox->checkState()==Qt::Checked) //если выбрана chekBox учесть
    {
        if(QSqlDatabase::contains("myDB"))
        {
            QSqlDatabase db = QSqlDatabase::database("myDB");
            db.setDatabaseName("/home/pi/chambers.db");
            db.open();
            QSqlQuery query(db);
            if(!query.exec("SELECT * FROM config"))
            {
                qDebug()<<"no selected";
            }
            if(query.first())
            {
                if(query.value("threshold_dose_dimension").toString()=="1")
                {
                    qDebug()<<"threshold_dose_dimension == 1";
                    QString update = "UPDATE config SET account_compensation='%1'";
                    QString update1 = update.arg("1");
                    //QSqlQuery query1(db);
                    if(!query.exec(update1))
                    {
                        qDebug()<<"unable execute query CHeckBOX";
                    }
                }
            }
            db.close();
        }
    }
    if(ui->checkBox->checkState()==Qt::Unchecked) //если не выбрана chekBox учесть
    {
        if(QSqlDatabase::contains("myDB"))
        {
            QSqlDatabase db = QSqlDatabase::database("myDB");
            db.setDatabaseName("/home/pi/chambers.db");
            db.open();
            QSqlQuery query(db);
            QString update = "UPDATE config SET account_compensation='%1'";
            QString update1 = update.arg("0");
            if(!query.exec(update1))
            {
                qDebug()<<"unable execute query CHeckBOX";
            }
            db.close();
        }
    }

    if(ui->checkBox_2->checkState()==Qt::Checked) //если выбрана chekBox-2 учесть
    {
        if(QSqlDatabase::contains("myDB"))
        {
            QSqlDatabase db = QSqlDatabase::database("myDB");
            db.setDatabaseName("/home/pi/chambers.db");
            db.open();
            QSqlQuery query(db);
            QString update = "UPDATE config SET add_filter='%1'";
            QString update1 = update.arg("1");
            if(!query.exec(update1))
            {
                qDebug()<<"unable execute query CHeckBOX";
            }
            db.close();
        }        
    }
    if(ui->checkBox_2->checkState()==Qt::Unchecked) //если не выбрана chekBox-2 учесть
    {
        if(QSqlDatabase::contains("myDB"))
        {
            QSqlDatabase db = QSqlDatabase::database("myDB");
            db.setDatabaseName("/home/pi/chambers.db");
            db.open();
            QSqlQuery query(db);
            QString update = "UPDATE config SET add_filter='%1'";
            QString update1 = update.arg("0");
            if(!query.exec(update1))
            {
                qDebug()<<"unable execute query CHeckBOX";
            }
            db.close();
        }        
    }

    if(ui->checkBox_3->checkState()==Qt::Checked) //если выбрана chekBox-3 учесть
    {
        if(QSqlDatabase::contains("myDB"))
        {
            QSqlDatabase db = QSqlDatabase::database("myDB");
            db.setDatabaseName("/home/pi/chambers.db");
            db.open();
            QSqlQuery query(db);
            QString update = "UPDATE config SET standart_deviation_show='%1'";
            QString update1 = update.arg("1");
            if(!query.exec(update1))
            {
                qDebug()<<"unable execute query CHeckBOX";
            }
            db.close();
        }
    }
    if(ui->checkBox_3->checkState()==Qt::Unchecked) //если не выбрана chekBox-3 учесть
    {
        if(QSqlDatabase::contains("myDB"))
        {
            QSqlDatabase db = QSqlDatabase::database("myDB");
            db.setDatabaseName("/home/pi/chambers.db");
            db.open();
            QSqlQuery query(db);
            QString update = "UPDATE config SET standart_deviation_show='%1'";
            QString update1 = update.arg("0");
            if(!query.exec(update1))
            {
                qDebug()<<"unable execute query CHeckBOX";
            }
            db.close();
        }
    }
    if(ui->comboBox_2->currentText()=="чувствительный")
    {
        if(QSqlDatabase::contains("myDB"))
        {
            QSqlDatabase db = QSqlDatabase::database("myDB");
            db.setDatabaseName("/home/pi/chambers.db");
            db.open();
            QSqlQuery query(db);
            QString update = "UPDATE config SET measurement_range='%1'"; // диапазон измерения на чувствительный
            QString update1 = update.arg("1");
            if(!query.exec(update1))
            {
                qDebug()<<"unable execute query threshold_dose_dimension";
            }
            db.close();
        }
    }
    if(ui->comboBox_2->currentText()=="средний")
    {
        if(QSqlDatabase::contains("myDB"))
        {
            QSqlDatabase db = QSqlDatabase::database("myDB");
            db.setDatabaseName("/home/pi/chambers.db");
            db.open();
            QSqlQuery query(db);
            QString update = "UPDATE config SET measurement_range='%1'"; // диапазон измерения на средний
            QString update1 = update.arg("2");
            if(!query.exec(update1))
            {
                qDebug()<<"unable execute query threshold_dose_dimension";
            }
            db.close();
        }
    }
    if(ui->comboBox_2->currentText()=="грубый")
    {
        if(QSqlDatabase::contains("myDB"))
        {
            QSqlDatabase db = QSqlDatabase::database("myDB");
            db.setDatabaseName("/home/pi/chambers.db");
            db.open();
            QSqlQuery query(db);
            QString update = "UPDATE config SET measurement_range='%1'"; // диапазон измерения на грубый
            QString update1 = update.arg("3");
            if(!query.exec(update1))
            {
                qDebug()<<"unable execute query threshold_dose_dimension";
            }
            db.close();
        }
    }
    emit updateMesPar();
    qDebug()<<"emit updateMesPar();";
    close();
    delete this;
}

void Measuring_parameters::on_pushButton_clicked() //кнопка выполнить компенсацию
{
   Compensation *compensation = new Compensation(this);
   compensation->setModal(true);
   compensation->showFullScreen();
   connect(this,SIGNAL(sendADCfromMesPar(QString)),compensation,SLOT(recieveADCfromMesPar(QString)));
   connect(compensation,SIGNAL(compPar(int,int,int)),this,SLOT(compPar(int,int,int)));
   connect(compensation,SIGNAL(sendByteToMesPar(QByteArray)),this,SLOT(recieveBytesFromCompensation(QByteArray)));
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("/home/pi/chambers.db");
        db.open();
        QSqlQuery query(db);
        QString update = "UPDATE config SET threshold_dose_dimension='%1'"; // флаг о выполнении компенсации в БД (в свободную threshold_dose_dimension)
        QString update1 = update.arg("1");
        if(!query.exec(update1))
        {
            qDebug()<<"unable execute query CHeckBOX";
        }
        db.close();
    }
}

void Measuring_parameters::recieveBytesFromCompensation(QByteArray data)
{
    emit sendBytesToMes(data);
}

void Measuring_parameters::compPar(int Comp1,int Comp2,int Comp3) //функция компенсации параметров
{
    double comp1=Comp1*0.0000003/(R3*1000000000);
    double comp2=Comp2*0.0000003/(R2*1000000);
    double comp3=Comp3*0.0000003/(R1*1000000);
    ui->label_4->setText(QString::number(comp1*1e15)+"пА");
    ui->label_5->setText(QString::number(comp2*1e15)+"пА");
    ui->label_6->setText(QString::number(comp3*1e15)+"пА");
    ui->label_7->setText("Учесть компенсацию?");

    if(QSqlDatabase::contains("myDB"))  // запись в БД коэффициентов для трех диапазонов
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("/home/pi/chambers.db");
        db.open();
        QSqlQuery query(db);
        QString update = "UPDATE config SET compensation1='%1', compensation2='%2', compensation3='%3';";
        QString update1 = update.arg(QString::number(Comp1))
                                .arg(QString::number(Comp2))
                                .arg(QString::number(Comp3));
        if(!query.exec(update1))
        {
            qDebug()<<"unable execute query CHeckBOX"<<query.lastError().text();
        }
        db.close();
    }
}


