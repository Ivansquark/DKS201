//***********************                                         *******************
//*********************** Самый большой виджет - легко запутаться *******************
//***********************                                         *******************

#include "measuring.h"
#include "ui_measuring.h"

Measuring::Measuring(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Measuring) // конструктор
{
    av = 0;
    otkl=0;
    Q=0;
    D=0;
    qDebug()<<"constructor mes";
    integral_I=0;

    ui->setupUi(this);

    CSbut=0;
    CS_flag=0;


    Time_threshold=0;DOSE_threshold=0;

    nullADC=8388607;

    connect(this,SIGNAL(threshold_stop()),this,SLOT(stopClick())); // connect на кнопку стоп по сигналу о срабатывании порога

//**************Connect на таймер -- из-за него иногда возникает SEGMENTATION FAULT хз почему-- (добавил в деструктор delete timer)***************

    connect(timer,SIGNAL(timeout()),this,SLOT(timeOut()));
    qDebug()<<"connect timer";
//********************************************************************************************************

    QStringList strList;
    QFile config("/home/pi/config.txt");
    if(config.exists()&&config.open(QIODevice::ReadOnly))
    {
        while(!config.atEnd())
        {
            strList.append(config.readLine());
        }
        config.close();
    }
    R1=strList[1].toDouble();
    C1=strList[2].toDouble();
    qDebug()<<R1<<C1;

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
    C2=strList1[3].toDouble();
    inI=strList1[4].toDouble();
    inU=inI;
    qDebug()<<R2<<R3<<C2<<inI<<inU;


    QSqlDatabase::addDatabase("QSQLITE","myDB");  //форма по значениям из БД

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
            qDebug()<<"unable execute queryyyy";
        }
        if(query.first())
        {            
            dimensionSensibility=query.value("dimensionSensibility").toString().remove(0,1);
            measuring_parameter_dimension=query.value("measuring_parameter_dimension").toString();
            pressure = query.value("pressure").toDouble();
            temperature = query.value("temperature").toDouble();
            correctionF = query.value("correction_factor").toDouble();
            sensibility = query.value("sensibility").toDouble()*1e6;
            measuring_parameter = query.value("measuring_parameter").toString();
            measurement_range = query.value("measurement_range").toString();
            standart_deviation_show=query.value("standart_deviation_show").toString();


            qDebug()<<dimensionSensibility<<measuring_parameter_dimension<<measuring_parameter<<pressure<<temperature<<correctionF<<sensibility;

            ui->label_15->setText(query.value("name").toString());
            ui->label_16->setText(query.value("type").toString());
            ui->label_17->setText(query.value("volume").toString());
            ui->label_19->setText(query.value("voltage").toString());
            if(query.value("measurement_range").toString()=="1")
            {
               ui->label_6->setText("Чувствительный");
            }
            if(query.value("measurement_range").toString()=="2")
            {
               ui->label_6->setText("Средний");
            }
            if(query.value("measurement_range").toString()=="3")
            {
               ui->label_6->setText("Грубый");
            }
            //**************** Установка значения измерения ******************************************************
            if(query.value("measuring_parameter").toString()=="1")
            {
                ui->label_5->setText("Ток");
                ui->label_9->setText("Интеграл тока");
                ui->label_8->setText("А");
                ui->label_11->setText("Кл");
                if(query.value("standart_deviation_show")=="0")
                {
                    ui->label_13->setText("");
                    ui->label_14->setText("");
                    ui->label_20->setText("");
                    ui->label_21->setText("");
                    ui->label_12->setText("");
                }
                if(query.value("standart_deviation_show")=="1")
                {
                    ui->label_13->setText("СКО= ");
                    ui->label_14->setText("?");
                    ui->label_20->setText("<");
                    ui->label_21->setText(">");
                    ui->label_12->setText("0.000");
                }
            }
            if(query.value("measuring_parameter").toString()=="2")
            {
                ui->label_5->setText("Интеграл тока");
                ui->label_9->setText("Ток");
                ui->label_11->setText("А");
                ui->label_8->setText("Кл");

                ui->label_13->setText("");
                ui->label_14->setText("");
                ui->label_20->setText("");
                ui->label_21->setText("");
                ui->label_12->setText("");
            }
            if(query.value("measuring_parameter").toString()=="3")
            {
                ui->label_5->setText("Заряд");
                ui->label_9->setText("Ток");
                ui->label_11->setText("А");
                ui->label_8->setText("Кл");

                ui->label_13->setText("");
                ui->label_14->setText("");
                ui->label_20->setText("");
                ui->label_21->setText("");
                ui->label_12->setText("");
            }
            if(query.value("measuring_parameter").toString()=="4"&&query.value("dimensionSensibility").toString()=="мГр")
            {
                ui->label_5->setText("МПД");
                ui->label_9->setText("Интеграл МПД");
                ui->label_8->setText(query.value("measuring_parameter_dimension").toString());
                ui->label_11->setText(query.value("dimensionSensibility").toString());
                if(query.value("standart_deviation_show")=="0")
                {
                    ui->label_13->setText("");
                    ui->label_14->setText("");
                    ui->label_20->setText("");
                    ui->label_21->setText("");
                    ui->label_12->setText("");
                }
                if(query.value("standart_deviation_show")=="1")
                {
                    ui->label_13->setText("СКО= ");
                    ui->label_14->setText("?");
                    ui->label_20->setText("<");
                    ui->label_21->setText(">");
                    ui->label_12->setText("0.000");
                }
            }
            if(query.value("measuring_parameter").toString()=="4"&&query.value("dimensionSensibility").toString()=="мЗв")
            {
                ui->label_5->setText("МАЭД");
                ui->label_9->setText("Интеграл МАЭД");
                ui->label_8->setText(query.value("measuring_parameter_dimension").toString());
                ui->label_11->setText(query.value("dimensionSensibility").toString());
                if(query.value("standart_deviation_show")=="0")
                {
                    ui->label_13->setText("");
                    ui->label_14->setText("");
                    ui->label_20->setText("");
                    ui->label_21->setText("");
                    ui->label_12->setText("");
                }
                if(query.value("standart_deviation_show")=="1")
                {
                    ui->label_13->setText("СКО= ");
                    ui->label_14->setText("?");
                    ui->label_20->setText("<");
                    ui->label_21->setText(">");
                    ui->label_12->setText("0.000");
                }
            }
            if(query.value("measuring_parameter").toString()=="4"&&query.value("dimensionSensibility").toString()=="мР")
            {
                ui->label_5->setText("МЭД");
                ui->label_9->setText("МЭД");
                ui->label_8->setText(query.value("measuring_parameter_dimension").toString());
                ui->label_11->setText(query.value("dimensionSensibility").toString());
                if(query.value("standart_deviation_show")=="0")
                {
                    ui->label_13->setText("");
                    ui->label_14->setText("");
                    ui->label_20->setText("");
                    ui->label_21->setText("");
                    ui->label_12->setText("");
                }
                if(query.value("standart_deviation_show")=="1")
                {
                    ui->label_13->setText("СКО= ");
                    ui->label_14->setText("?");
                    ui->label_20->setText("<");
                    ui->label_21->setText(">");
                    ui->label_12->setText("0.000");
                }
            }
            if(query.value("measuring_parameter").toString()=="5"&&query.value("dimensionSensibility").toString()=="мГр")
            {
                ui->label_5->setText("Интеграл МПД");
                ui->label_9->setText("МПД");
                ui->label_11->setText(query.value("measuring_parameter_dimension").toString());
                ui->label_8->setText(query.value("dimensionSensibility").toString());

                ui->label_13->setText("");
                ui->label_14->setText("");
                ui->label_20->setText("");
                ui->label_21->setText("");
                ui->label_12->setText("");
            }
            if(query.value("measuring_parameter").toString()=="5"&&query.value("dimensionSensibility").toString()=="мЗв")
            {
                ui->label_5->setText("Интеграл МАЭД");
                ui->label_9->setText("МАЭД");
                ui->label_11->setText(query.value("measuring_parameter_dimension").toString());
                ui->label_8->setText(query.value("dimensionSensibility").toString());

                ui->label_13->setText("");
                ui->label_14->setText("");
                ui->label_20->setText("");
                ui->label_21->setText("");
                ui->label_12->setText("");
            }
            if(query.value("measuring_parameter").toString()=="5"&&query.value("dimensionSensibility").toString()=="мР")
            {
                ui->label_5->setText("Интеграл МЭД");
                ui->label_9->setText("МЭД");
                ui->label_11->setText(query.value("measuring_parameter_dimension").toString());
                ui->label_8->setText(query.value("dimensionSensibility").toString());

                ui->label_13->setText("");
                ui->label_14->setText("");
                ui->label_20->setText("");
                ui->label_21->setText("");
                ui->label_12->setText("");
            }
            if(query.value("measuring_parameter").toString()=="6"&&query.value("dimensionSensibility").toString()=="мГр")
            {
                ui->label_5->setText("ПД");
                ui->label_9->setText("МПД");
                ui->label_8->setText(query.value("dimensionSensibility").toString());
                ui->label_11->setText(query.value("measuring_parameter_dimension").toString());

                ui->label_13->setText("");
                ui->label_14->setText("");
                ui->label_20->setText("");
                ui->label_21->setText("");
                ui->label_12->setText("");
            }
            if(query.value("measuring_parameter").toString()=="6"&&query.value("dimensionSensibility").toString()=="мЗв")
            {
                ui->label_5->setText("АЭД");
                ui->label_9->setText("МАЭД");
                ui->label_8->setText(query.value("dimensionSensibility").toString());
                ui->label_11->setText(query.value("measuring_parameter_dimension").toString());

                ui->label_13->setText("");
                ui->label_14->setText("");
                ui->label_20->setText("");
                ui->label_21->setText("");
                ui->label_12->setText("");
            }
            if(query.value("measuring_parameter").toString()=="6"&&query.value("dimensionSensibility").toString()=="мР")
            {
                ui->label_5->setText("ЭксД");
                ui->label_9->setText("МЭД");
                ui->label_8->setText(query.value("dimensionSensibility").toString());
                ui->label_11->setText(query.value("measuring_parameter_dimension").toString());

                ui->label_13->setText("");
                ui->label_14->setText("");
                ui->label_20->setText("");
                ui->label_21->setText("");
                ui->label_12->setText("");
            }
        }
    }
}

Measuring::~Measuring() //деструктор
{
    delete ui;
    delete timer;
    qDebug()<<"destruptor Mes";
    emit close2();
    emit closePortTest();
}
void Measuring::stopClick()  //функция приема сигнала о достижении порога и нажатия на клавишу стоп
{
    ui->pushButton_8->click();
}

void Measuring::on_pushButton_6_clicked() //кнопка выключение (выставление флагов сброса некоторых параметров в БД)
{
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("/home/pi/chambers.db");
        db.open();
        QSqlQuery query(db);
        QString update = "UPDATE config SET measurement_mode='%1', account_compensation='%2', measurement_range='%3'";
        QString update1 = update.arg("manual").arg("0").arg("1");
        if(!query.exec(update1))
        {
            qDebug()<<"unable execute query update onEXIT";
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
    close();
    //QByteArray NullByteVolt={0x78,static_cast<char>(0x80)}; // выставление нулевого напряжения при выключении
    QByteArray NullByteVolt;
    NullByteVolt[0]=0x78;
    NullByteVolt[1]=static_cast<char>(0x80);
    emit bytesVoltM(NullByteVolt); //отсылка сигнала с нулевым напряжением
    emit bytesVoltM(NullByteVolt); //отсылка сигнала с нулевым напряжением
    delete this;
}

void Measuring::on_pushButton_5_clicked() // кнопка настройки режима измерения (ручной, автоматический)
{
    Measuring_mode *mes_mode = new Measuring_mode(this);
    mes_mode->setModal(true);
    mes_mode->showFullScreen();
    connect(mes_mode,SIGNAL(sendStr(QString)),this,SLOT(recieveStr(QString)));
}

void Measuring::recieveStr(QString str) // прием сигнала от окна (ручной - автоматический) (пока недаписано!!!)
{
    ui->label->setText(str);
}

void Measuring::on_pushButton_2_clicked() //кнопка параметры создание объекта и коннекта
{
    ui->label_23->setText("");
    Measuring_parameters *mesPar = new Measuring_parameters(this);
    mesPar->setModal(true);
    if (CS_flag==0)
    {
        mesPar->init();
    }
    else
    {
       mesPar->init_CS();
    }
    mesPar->showFullScreen();
    connect(mesPar,SIGNAL(updateMesPar()),this,SLOT(updateMesPar()));
    connect(mesPar,SIGNAL(sendToPortMes(QByteArray)),this,SLOT(sendToPortMes(QByteArray)));
    connect(this,SIGNAL(sendADCtoMesPar(QString)),mesPar,SLOT(recieveADCtoMesPar(QString)));
    connect(mesPar,SIGNAL(sendBytesToMes(QByteArray)),this,SLOT(sendBytesFromMes(QByteArray)));// прием сигнала и пересылка в порт через MainWindow

}
void Measuring::sendBytesFromMes(QByteArray data)
{
    emit sendToPortMainW(data);
}
void Measuring::sendToPortMes(QByteArray data) // прием сигнала и пересылка в порт через MainWindow
{
    emit sendToPortMainW(data);
}

void Measuring::updateMesPar() // прием сигнала от кнопки ОК окна параметры (обновление формы по БД)
{
    ui->label_27->setText("");
    ui->label_28->setText("");
    ui->label_7->setText("0");
    ui->label_10->setText("0");
    ui->label_29->setText("");
    ui->label_30->setText("");
    D=0;
    Q=0;
    av = 0;
    otkl=0;
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db=QSqlDatabase::database("myDB");
        db.setDatabaseName("/home/pi/chambers.db");
        db.open();
        QSqlQuery query(db);
        if(!query.exec("SELECT *FROM config"))
        {
            qDebug()<<"unable execute query";
        }
        if(query.first())
        {
            dimensionSensibility=query.value("dimensionSensibility").toString().remove(0,1);
            measuring_parameter_dimension=query.value("measuring_parameter_dimension").toString();
            pressure = query.value("pressure").toDouble();
            temperature = query.value("temperature").toDouble();
            correctionF = query.value("correction_factor").toDouble();
            sensibility = query.value("sensibility").toDouble()*1e6;
            measuring_parameter = query.value("measuring_parameter").toString();
            measurement_range = query.value("measurement_range").toString();
            standart_deviation_show=query.value("standart_deviation_show").toString();

            ui->label_8->setText(query.value("measuring_parameter_dimension").toString());
            if(query.value("measurement_range").toString()=="1")
            {
               ui->label_6->setText("Чувствительный");
            }
            if(query.value("measurement_range").toString()=="2")
            {
               ui->label_6->setText("Средний");
            }
            if(query.value("measurement_range").toString()=="3")
            {
               ui->label_6->setText("Грубый");
            }

            //**************** Установка значения измерения ******************************************************
            if(query.value("measuring_parameter").toString()=="1")
            {
                ui->label_5->setText("Ток");
                ui->label_9->setText("Интеграл тока");
                ui->label_8->setText("А");
                ui->label_11->setText("Кл");
                if(query.value("standart_deviation_show")=="0")
                {
                    ui->label_13->setText("");
                    ui->label_14->setText("");
                    ui->label_20->setText("");
                    ui->label_21->setText("");
                    ui->label_12->setText("");
                }
                if(query.value("standart_deviation_show")=="1")
                {
                    ui->label_13->setText("СКО= ");
                    ui->label_14->setText("?");
                    ui->label_20->setText("<");
                    ui->label_21->setText(">");
                    ui->label_12->setText("0.000");
                }
            }
            if(query.value("measuring_parameter").toString()=="2")
            {
                ui->label_5->setText("Интеграл тока");
                ui->label_9->setText("Ток");
                ui->label_11->setText("А");
                ui->label_8->setText("Кл");

                ui->label_13->setText("");
                ui->label_14->setText("");
                ui->label_20->setText("");
                ui->label_21->setText("");
                ui->label_12->setText("");
            }
            if(query.value("measuring_parameter").toString()=="3")
            {
                ui->label_5->setText("Заряд");
                ui->label_9->setText("Ток");
                ui->label_11->setText("А");
                ui->label_8->setText("Кл");

                ui->label_13->setText("");
                ui->label_14->setText("");
                ui->label_20->setText("");
                ui->label_21->setText("");
                ui->label_12->setText("");
            }
            if(query.value("measuring_parameter").toString()=="4"&&query.value("dimensionSensibility").toString()=="мГр")
            {
                ui->label_5->setText("МПД");
                ui->label_9->setText("Интеграл МПД");
                ui->label_8->setText(query.value("measuring_parameter_dimension").toString());
                ui->label_11->setText(query.value("dimensionSensibility").toString());
                if(query.value("standart_deviation_show")=="0")
                {
                    ui->label_13->setText("");
                    ui->label_14->setText("");
                    ui->label_20->setText("");
                    ui->label_21->setText("");
                    ui->label_12->setText("");
                }
                if(query.value("standart_deviation_show")=="1")
                {
                    ui->label_13->setText("СКО= ");
                    ui->label_14->setText("?");
                    ui->label_20->setText("<");
                    ui->label_21->setText(">");
                    ui->label_12->setText("0.000");
                }
            }
            if(query.value("measuring_parameter").toString()=="4"&&query.value("dimensionSensibility").toString()=="мЗв")
            {
                ui->label_5->setText("МАЭД");
                ui->label_9->setText("Интеграл МАЭД");
                ui->label_8->setText(query.value("measuring_parameter_dimension").toString());
                ui->label_11->setText(query.value("dimensionSensibility").toString());
                if(query.value("standart_deviation_show")=="0")
                {
                    ui->label_13->setText("");
                    ui->label_14->setText("");
                    ui->label_20->setText("");
                    ui->label_21->setText("");
                    ui->label_12->setText("");
                }
                if(query.value("standart_deviation_show")=="1")
                {
                    ui->label_13->setText("СКО= ");
                    ui->label_14->setText("?");
                    ui->label_20->setText("<");
                    ui->label_21->setText(">");
                    ui->label_12->setText("0.000");
                }
            }
            if(query.value("measuring_parameter").toString()=="4"&&query.value("dimensionSensibility").toString()=="мР")
            {
                ui->label_5->setText("МЭД");
                ui->label_9->setText("МЭД");
                ui->label_8->setText(query.value("measuring_parameter_dimension").toString());
                ui->label_11->setText(query.value("dimensionSensibility").toString());
                if(query.value("standart_deviation_show")=="0")
                {
                    ui->label_13->setText("");
                    ui->label_14->setText("");
                    ui->label_20->setText("");
                    ui->label_21->setText("");
                    ui->label_12->setText("");
                }
                if(query.value("standart_deviation_show")=="1")
                {
                    ui->label_13->setText("СКО= ");
                    ui->label_14->setText("?");
                    ui->label_20->setText("<");
                    ui->label_21->setText(">");
                    ui->label_12->setText("0.000");
                }
            }
            if(query.value("measuring_parameter").toString()=="5"&&query.value("dimensionSensibility").toString()=="мГр")
            {
                ui->label_5->setText("Интеграл МПД");
                ui->label_9->setText("МПД");
                ui->label_11->setText(query.value("measuring_parameter_dimension").toString());
                ui->label_8->setText(query.value("dimensionSensibility").toString());

                ui->label_13->setText("");
                ui->label_14->setText("");
                ui->label_20->setText("");
                ui->label_21->setText("");
                ui->label_12->setText("");
            }
            if(query.value("measuring_parameter").toString()=="5"&&query.value("dimensionSensibility").toString()=="мЗв")
            {
                ui->label_5->setText("Интеграл МАЭД");
                ui->label_9->setText("МАЭД");
                ui->label_11->setText(query.value("measuring_parameter_dimension").toString());
                ui->label_8->setText(query.value("dimensionSensibility").toString());

                ui->label_13->setText("");
                ui->label_14->setText("");
                ui->label_20->setText("");
                ui->label_21->setText("");
                ui->label_12->setText("");
            }
            if(query.value("measuring_parameter").toString()=="5"&&query.value("dimensionSensibility").toString()=="мР")
            {
                ui->label_5->setText("Интеграл МЭД");
                ui->label_9->setText("МЭД");
                ui->label_11->setText(query.value("measuring_parameter_dimension").toString());
                ui->label_8->setText(query.value("dimensionSensibility").toString());

                ui->label_13->setText("");
                ui->label_14->setText("");
                ui->label_20->setText("");
                ui->label_21->setText("");
                ui->label_12->setText("");
            }
            if(query.value("measuring_parameter").toString()=="6"&&query.value("dimensionSensibility").toString()=="мГр")
            {
                ui->label_5->setText("ПД");
                ui->label_9->setText("МПД");
                ui->label_8->setText(query.value("dimensionSensibility").toString());
                ui->label_11->setText(query.value("measuring_parameter_dimension").toString());

                ui->label_13->setText("");
                ui->label_14->setText("");
                ui->label_20->setText("");
                ui->label_21->setText("");
                ui->label_12->setText("");
            }
            if(query.value("measuring_parameter").toString()=="6"&&query.value("dimensionSensibility").toString()=="мЗв")
            {
                ui->label_5->setText("АЭД");
                ui->label_9->setText("МАЭД");
                ui->label_8->setText(query.value("dimensionSensibility").toString());
                ui->label_11->setText(query.value("measuring_parameter_dimension").toString());

                ui->label_13->setText("");
                ui->label_14->setText("");
                ui->label_20->setText("");
                ui->label_21->setText("");
                ui->label_12->setText("");
            }
            if(query.value("measuring_parameter").toString()=="6"&&query.value("dimensionSensibility").toString()=="мР")
            {
                ui->label_5->setText("ЭксД");
                ui->label_9->setText("МЭД");
                ui->label_8->setText(query.value("dimensionSensibility").toString());
                ui->label_11->setText(query.value("measuring_parameter_dimension").toString());

                ui->label_13->setText("");
                ui->label_14->setText("");
                ui->label_20->setText("");
                ui->label_21->setText("");
                ui->label_12->setText("");
            }
        }
    }
}

void Measuring::on_pushButton_clicked() // кнопка установки - создание объекта и коннекта
{
    Measurement_config *mesConf = new Measurement_config(this);
    mesConf->setModal(true);
    mesConf->showFullScreen();
    connect(mesConf,SIGNAL(updateMes()),this,SLOT(updateMes()));
    connect(mesConf,SIGNAL(bytesVolt1(QByteArray)),this,SLOT(bytesVolt1(QByteArray)));
}
void Measuring::bytesVolt1(QByteArray data) //пересылка в порт байтов с высоким напряжением
{
    emit bytesVoltM(data);
}

void Measuring::updateMes() // прием сигнала от кнопки ОК окна УСТАНОВКИ (обновление формы по БД)
{
    ui->label_27->setText("");
    D=0;
    Q=0;
    av = 0;
    otkl=0;
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db=QSqlDatabase::database("myDB");
        db.setDatabaseName("/home/pi/chambers.db");
        db.open();
        QSqlQuery query(db);
        if(!query.exec("SELECT *FROM config"))
        {
            qDebug()<<"unable execute query";
        }
        if(query.first())
        {
            dimensionSensibility=query.value("dimensionSensibility").toString().remove(0,1);
            measuring_parameter_dimension=query.value("measuring_parameter_dimension").toString();
            pressure = query.value("pressure").toDouble();
            temperature = query.value("temperature").toDouble();
            correctionF = query.value("correction_factor").toDouble();
            sensibility = query.value("sensibility").toDouble()*1e6;
            measuring_parameter = query.value("measuring_parameter").toString();
            measurement_range = query.value("measurement_range").toString();
            standart_deviation_show=query.value("standart_deviation_show").toString();

            ui->label_15->setText(query.value("name").toString());
            ui->label_16->setText(query.value("type").toString());
            ui->label_17->setText(query.value("volume").toString());
            ui->label_19->setText(query.value("voltage").toString());

            //**************** Установка значения измерения ******************************************************
            if(query.value("measuring_parameter").toString()=="1")
            {
                ui->label_5->setText("Ток");
                ui->label_9->setText("Интеграл тока");
                ui->label_8->setText("А");
                ui->label_11->setText("Кл");
                if(query.value("standart_deviation_show")=="0")
                {
                    ui->label_13->setText("");
                    ui->label_14->setText("");
                    ui->label_20->setText("");
                    ui->label_21->setText("");
                    ui->label_12->setText("");
                }
                if(query.value("standart_deviation_show")=="1")
                {
                    ui->label_13->setText("СКО= ");
                    ui->label_14->setText("?");
                    ui->label_20->setText("<");
                    ui->label_21->setText(">");
                    ui->label_12->setText("0.000");
                }
            }
            if(query.value("measuring_parameter").toString()=="2")
            {
                ui->label_5->setText("Интеграл тока");
                ui->label_9->setText("Ток");
                ui->label_11->setText("А");
                ui->label_8->setText("Кл");

                ui->label_13->setText("");
                ui->label_14->setText("");
                ui->label_20->setText("");
                ui->label_21->setText("");
                ui->label_12->setText("");
            }
            if(query.value("measuring_parameter").toString()=="3")
            {
                ui->label_5->setText("Заряд");
                ui->label_9->setText("Ток");
                ui->label_11->setText("А");
                ui->label_8->setText("Кл");

                ui->label_13->setText("");
                ui->label_14->setText("");
                ui->label_20->setText("");
                ui->label_21->setText("");
                ui->label_12->setText("");
            }
            if(query.value("measuring_parameter").toString()=="4"&&query.value("dimensionSensibility").toString()=="мГр")
            {
                ui->label_5->setText("МПД");
                ui->label_9->setText("Интеграл МПД");
                ui->label_8->setText(query.value("measuring_parameter_dimension").toString());
                ui->label_11->setText(query.value("dimensionSensibility").toString());
                if(query.value("standart_deviation_show")=="0")
                {
                    ui->label_13->setText("");
                    ui->label_14->setText("");
                    ui->label_20->setText("");
                    ui->label_21->setText("");
                    ui->label_12->setText("");
                }
                if(query.value("standart_deviation_show")=="1")
                {
                    ui->label_13->setText("СКО= ");
                    ui->label_14->setText("?");
                    ui->label_20->setText("<");
                    ui->label_21->setText(">");
                    ui->label_12->setText("0.000");
                }
            }
            if(query.value("measuring_parameter").toString()=="4"&&query.value("dimensionSensibility").toString()=="мЗв")
            {
                ui->label_5->setText("МАЭД");
                ui->label_9->setText("Интеграл МАЭД");
                ui->label_8->setText(query.value("measuring_parameter_dimension").toString());
                ui->label_11->setText(query.value("dimensionSensibility").toString());
                if(query.value("standart_deviation_show")=="0")
                {
                    ui->label_13->setText("");
                    ui->label_14->setText("");
                    ui->label_20->setText("");
                    ui->label_21->setText("");
                    ui->label_12->setText("");
                }
                if(query.value("standart_deviation_show")=="1")
                {
                    ui->label_13->setText("СКО= ");
                    ui->label_14->setText("?");
                    ui->label_20->setText("<");
                    ui->label_21->setText(">");
                    ui->label_12->setText("0.000");
                }
            }
            if(query.value("measuring_parameter").toString()=="4"&&query.value("dimensionSensibility").toString()=="мР")
            {
                ui->label_5->setText("МЭД");
                ui->label_9->setText("МЭД");
                ui->label_8->setText(query.value("measuring_parameter_dimension").toString());
                ui->label_11->setText(query.value("dimensionSensibility").toString());
                if(query.value("standart_deviation_show")=="0")
                {
                    ui->label_13->setText("");
                    ui->label_14->setText("");
                    ui->label_20->setText("");
                    ui->label_21->setText("");
                    ui->label_12->setText("");
                }
                if(query.value("standart_deviation_show")=="1")
                {
                    ui->label_13->setText("СКО= ");
                    ui->label_14->setText("?");
                    ui->label_20->setText("<");
                    ui->label_21->setText(">");
                    ui->label_12->setText("0.000");
                }
            }
            if(query.value("measuring_parameter").toString()=="5"&&query.value("dimensionSensibility").toString()=="мГр")
            {
                ui->label_5->setText("Интеграл МПД");
                ui->label_9->setText("МПД");
                ui->label_11->setText(query.value("measuring_parameter_dimension").toString());
                ui->label_8->setText(query.value("dimensionSensibility").toString());

                ui->label_13->setText("");
                ui->label_14->setText("");
                ui->label_20->setText("");
                ui->label_21->setText("");
                ui->label_12->setText("");
            }
            if(query.value("measuring_parameter").toString()=="5"&&query.value("dimensionSensibility").toString()=="мЗв")
            {
                ui->label_5->setText("Интеграл МАЭД");
                ui->label_9->setText("МАЭД");
                ui->label_11->setText(query.value("measuring_parameter_dimension").toString());
                ui->label_8->setText(query.value("dimensionSensibility").toString());

                ui->label_13->setText("");
                ui->label_14->setText("");
                ui->label_20->setText("");
                ui->label_21->setText("");
                ui->label_12->setText("");
            }
            if(query.value("measuring_parameter").toString()=="5"&&query.value("dimensionSensibility").toString()=="мР")
            {
                ui->label_5->setText("Интеграл МЭД");
                ui->label_9->setText("МЭД");
                ui->label_11->setText(query.value("measuring_parameter_dimension").toString());
                ui->label_8->setText(query.value("dimensionSensibility").toString());

                ui->label_13->setText("");
                ui->label_14->setText("");
                ui->label_20->setText("");
                ui->label_21->setText("");
                ui->label_12->setText("");
            }
            if(query.value("measuring_parameter").toString()=="6"&&query.value("dimensionSensibility").toString()=="мГр")
            {
                ui->label_5->setText("ПД");
                ui->label_9->setText("МПД");
                ui->label_8->setText(query.value("dimensionSensibility").toString());
                ui->label_11->setText(query.value("measuring_parameter_dimension").toString());

                ui->label_13->setText("");
                ui->label_14->setText("");
                ui->label_20->setText("");
                ui->label_21->setText("");
                ui->label_12->setText("");
            }
            if(query.value("measuring_parameter").toString()=="6"&&query.value("dimensionSensibility").toString()=="мЗв")
            {
                ui->label_5->setText("АЭД");
                ui->label_9->setText("МАЭД");
                ui->label_8->setText(query.value("dimensionSensibility").toString());
                ui->label_11->setText(query.value("measuring_parameter_dimension").toString());

                ui->label_13->setText("");
                ui->label_14->setText("");
                ui->label_20->setText("");
                ui->label_21->setText("");
                ui->label_12->setText("");
            }
            if(query.value("measuring_parameter").toString()=="6"&&query.value("dimensionSensibility").toString()=="мР")
            {
                ui->label_5->setText("ЭксД");
                ui->label_9->setText("МЭД");
                ui->label_8->setText(query.value("dimensionSensibility").toString());
                ui->label_11->setText(query.value("measuring_parameter_dimension").toString());

                ui->label_13->setText("");
                ui->label_14->setText("");
                ui->label_20->setText("");
                ui->label_21->setText("");
                ui->label_12->setText("");
            }
        }
    }
}

void Measuring::recieveADC(QString str) // функция срабатывающая по приему данных (в эмуляторе по приему от класса avrADC)
{    
    avrADC = str.toLong();
    ADC_V = (avrADC-nullADC)*0.0000003*1*(-1); // - ускорим в ххх раз

        ui->label_22->setText(QString::number(ADC_V,'f',6));
        emit sendADCtoMesPar(str);
}


void Measuring::recieveADC_COM(QString str) // функция срабатывающая по приему данных (от класса avrADC от СОМ порта)
{

    avrADC = str.toLong();

    ADC_V = (avrADC-inU-nullADC)*0.0000003;
        ui->label_22->setText(QString::number(ADC_V,'f',6));    
        emit sendADCtoMesPar(str);
}


//************************ Вспомогательные функции вывода на дисплей цифр в четырехзначном представлении ******************************
void Measuring::displaingMes_label_7(double z1)
{
    if (z1<0) ui->label_28->setText("-");
    else ui->label_28->setText("");
    double z = abs(z1);
    if (QString::number(z,'f',3).size()<6)
    {
        ui->label_7->setText(QString::number(z,'f',3));  // отображение
    }
    else if (QString::number(z,'f',3).size()>=6 && QString::number(z,'f',3).size()<7)
    {
        ui->label_7->setText(QString::number(z,'f',2));  // отображение
    }
    else if (QString::number(z,'f',3).size()>=7 && QString::number(z,'f',3).size()<8)
    {
        ui->label_7->setText(QString::number(z,'f',1));  // отображение
    }
    else if (QString::number(z,'f',3).size()>=8)
    {
        ui->label_7->setText(QString::number(z,'f',0));  // отображение
    }
}
void Measuring::displaingMes_label_10(double z1)
{
    if (z1<0) ui->label_29->setText("-");
    else ui->label_29->setText("");
    double z = abs(z1);
    if (QString::number(z,'f',3).size()<6)
    {
        ui->label_10->setText(QString::number(z,'f',3));  // отображение
    }
    else if (QString::number(z,'f',3).size()>=6 && QString::number(z,'f',3).size()<7)
    {
        ui->label_10->setText(QString::number(z,'f',2));  // отображение
    }
    else if (QString::number(z,'f',3).size()>=7 && QString::number(z,'f',3).size()<8)
    {
        ui->label_10->setText(QString::number(z,'f',1));  // отображение
    }
    else if (QString::number(z,'f',3).size()>=8)
    {
        ui->label_10->setText(QString::number(z,'f',0));  // отображение
    }
}
void Measuring::displaingMes_label_12(double z1)
{
    if (z1<0) ui->label_30->setText("-");
    else ui->label_30->setText("");
    double z = abs(z1);
    if (QString::number(z,'f',3).size()<6)
    {
        ui->label_12->setText(QString::number(z,'f',3));  // отображение
    }
    else if (QString::number(z,'f',3).size()>=6 && QString::number(z,'f',3).size()<7)
    {
        ui->label_12->setText(QString::number(z,'f',2));  // отображение
    }
    else if (QString::number(z,'f',3).size()>=7 && QString::number(z,'f',3).size()<8)
    {
        ui->label_12->setText(QString::number(z,'f',1));  // отображение
    }
    else if (QString::number(z,'f',3).size()>=8)
    {
        ui->label_12->setText(QString::number(z,'f',0));  // отображение
    }
}
void Measuring::displaingMes_label_14(double z1)
{
    //if (z1<0) ui->label_30->setText("-");
    //else ui->label_30->setText("");
    double z = abs(z1);
    if (QString::number(z,'f',3).size()<6)
    {
        ui->label_14->setText(QString::number(z,'f',3));  // отображение
    }
    else if (QString::number(z,'f',3).size()>=6 && QString::number(z,'f',3).size()<7)
    {
        ui->label_14->setText(QString::number(z,'f',2));  // отображение
    }
    else if (QString::number(z,'f',3).size()>=7 && QString::number(z,'f',3).size()<8)
    {
        ui->label_14->setText(QString::number(z,'f',1));  // отображение
    }
    else if (QString::number(z,'f',3).size()>=8)
    {
        ui->label_14->setText(QString::number(z,'f',0));  // отображение
    }
}

//************************************************************************************************************************************

void Measuring::on_pushButton_7_clicked() //кнопка старт
{
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_4->setStyleSheet("color:grey");
    ui->pushButton_5->setEnabled(false);
    ui->pushButton_7->setEnabled(false);

    time=0;
    Time_threshold=0;DOSE_threshold=0;
    //timer=nullptr;
    ui->label_4->setText(QString::number(time));
    timer->start(1000);
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("/home/pi/chambers.db");
        QSqlQuery query(db);
        if(!query.exec("SELECT *FROM config"))
        {
            qDebug()<<"Unable execute query charge";
        }
        if (query.first())
        {
            if((query.value("measuring_parameter").toString()=="3"||query.value("measuring_parameter").toString()=="6")
              &&query.value("measurement_range").toString()=="1") //заряд чувствительный
            {
                QByteArray h80;
                h80[0]=0x28;//28
                h80[1]=0x30;//30 код реле - К7+К8 - заряд (по кнопке старт) в режиме измерения заряда на чувствительном диапазоне
                emit sendToPortMainW(h80);
                emit sendToPortMainW(h80);
            }
            if((query.value("measuring_parameter").toString()=="3"||query.value("measuring_parameter").toString()=="6")
              &&query.value("measurement_range").toString()=="2") //заряд средний
            {
                QByteArray hC0;
                hC0[0]=0x2c;//2c
                hC0[1]=0x30;//30 код реле - К6+К8 - заряд (по кнопке старт) в режиме измерения заряда на среднем и грубом диапазоне
                emit sendToPortMainW(hC0);
                emit sendToPortMainW(hC0);
            }
            if((query.value("measuring_parameter").toString()=="3"||query.value("measuring_parameter").toString()=="6")
              &&query.value("measurement_range").toString()=="3") //заряд грубый
            {
                QByteArray hC0;
                hC0[0]=0x2c;//2c
                hC0[1]=0x30;//30 код реле - К6+К8 - заряд (по кнопке старт) в режиме измерения заряда на среднем и грубом диапазоне
                emit sendToPortMainW(hC0);
                emit sendToPortMainW(hC0);
            }
            if((query.value("measuring_parameter").toString()=="1"||query.value("measuring_parameter").toString()=="2"||
               query.value("measuring_parameter").toString()=="4"||query.value("measuring_parameter").toString()=="5")&&
               query.value("measurement_range").toString()=="1"&&query.value("add_filter").toString()=="1") //ток(и т.д.)+чувствительный+фильтр
            {
                QByteArray hC2;
                hC2[0]=0x2c;//2c
                hC2[1]=0x32;//30 код реле - К6+К8 - заряд (по кнопке старт) в режиме измерения заряда на среднем и грубом диапазоне
                emit sendToPortMainW(hC2);
                emit sendToPortMainW(hC2);
            }
            if((query.value("measuring_parameter").toString()=="1"||query.value("measuring_parameter").toString()=="2"||
               query.value("measuring_parameter").toString()=="4"||query.value("measuring_parameter").toString()=="5")&&
               query.value("measurement_range").toString()=="1"&&query.value("add_filter").toString()=="0") //ток(и т.д.)+чувствительный
            {
                QByteArray h82;
                h82[0]=0x28;//2c
                h82[1]=0x32;//30 код реле - К6+К8 - заряд (по кнопке старт) в режиме измерения заряда на среднем и грубом диапазоне
                emit sendToPortMainW(h82);
                emit sendToPortMainW(h82);
            }
            if((query.value("measuring_parameter").toString()=="1"||query.value("measuring_parameter").toString()=="2"||
               query.value("measuring_parameter").toString()=="4"||query.value("measuring_parameter").toString()=="5")&&
               query.value("measurement_range").toString()=="2"&&query.value("add_filter").toString()=="1") //ток(и т.д.)+средний+фильтр
            {
                QByteArray h90;
                h90[0]=0x29;//2c
                h90[1]=0x30;//30 код реле - К6+К8 - заряд (по кнопке старт) в режиме измерения заряда на среднем и грубом диапазоне
                emit sendToPortMainW(h90);
                emit sendToPortMainW(h90);
            }
            if((query.value("measuring_parameter").toString()=="1"||query.value("measuring_parameter").toString()=="2"||
               query.value("measuring_parameter").toString()=="4"||query.value("measuring_parameter").toString()=="5")&&
               query.value("measurement_range").toString()=="2"&&query.value("add_filter").toString()=="0") //ток(и т.д.)+средний
            {
                QByteArray h90;
                h90[0]=0x29;//2c
                h90[1]=0x30;//30 код реле - К6+К8 - заряд (по кнопке старт) в режиме измерения заряда на среднем и грубом диапазоне
                emit sendToPortMainW(h90);
                emit sendToPortMainW(h90);
            }
            if((query.value("measuring_parameter").toString()=="1"||query.value("measuring_parameter").toString()=="2"||
               query.value("measuring_parameter").toString()=="4"||query.value("measuring_parameter").toString()=="5")&&
               query.value("measurement_range").toString()=="3"&&query.value("add_filter").toString()=="1") //ток(и т.д.)+грубый+фильтр
            {
                QByteArray hA0;
                hA0[0]=0x2A;//2c
                hA0[1]=0x30;//30 код реле - К6+К8 - заряд (по кнопке старт) в режиме измерения заряда на среднем и грубом диапазоне
                emit sendToPortMainW(hA0);
                emit sendToPortMainW(hA0);
            }
            if((query.value("measuring_parameter").toString()=="1"||query.value("measuring_parameter").toString()=="2"||
               query.value("measuring_parameter").toString()=="4"||query.value("measuring_parameter").toString()=="5")&&
               query.value("measurement_range").toString()=="3"&&query.value("add_filter").toString()=="0") //ток(и т.д.)+грубый
            {
                QByteArray hA0;
                hA0[0]=0x29;//2c
                hA0[1]=0x30;//30 код реле - К6+К8 - заряд (по кнопке старт) в режиме измерения заряда на среднем и грубом диапазоне
                emit sendToPortMainW(hA0);
                emit sendToPortMainW(hA0);
            }
            if(query.value("threshold").toString()=="time")
            {
                QString strTime = query.value("threshold_time").toString();
                Time_threshold = strTime.toInt();
                qDebug()<<"threashold_time Called"<<Time_threshold;
            }
            if(query.value("threshold").toString()=="dose")
            {
                QString strDOSE = query.value("threshold_dose").toString();
                DOSE_threshold = strDOSE.toDouble()/1000;
                qDebug()<<"threashold_dose Called"<<DOSE_threshold<<"Гр или Зв";
            }
        }
    }
}


double Measuring::compensation_ADC_V() // функция учитывающая выполненную компенсацию по диапазонам
{
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("/home/pi/chambers.db");
        QSqlQuery query(db);
        if(!query.exec("SELECT *FROM config"))
        {
            qDebug()<<"unable execute query - SELECT";
        }
        if(query.first())
        {
            if(query.value("account_compensation").toString()=="0") // не учитываются
            {
                return ADC_V;
            }
            if(query.value("account_compensation").toString()=="1") // учитывается
            {
                if(query.value("measurement_range").toString()=="1") // Чувствительный
                {
                    double comp1=query.value("compensation1").toDouble();
                    ADC_V=(avrADC-inU-nullADC-comp1)*0.0000003;
                    return ADC_V;
                }
                if(query.value("measurement_range").toString()=="2") // Средний
                {
                    double comp2=query.value("compensation2").toDouble();
                    ADC_V=(avrADC-inU-nullADC-comp2)*0.0000003;
                    return ADC_V;
                }
                if(query.value("measurement_range").toString()=="3") // Грубый
                {
                    double comp3=query.value("compensation3").toDouble();
                    ADC_V=(avrADC-inU-nullADC-comp3)*0.0000003;
                    return ADC_V;
                }
            }
        }
    }
    qDebug()<<"ERRRRRORRRRRR0";
    return 0;
}


//****************************************************************************************************************************
//************************   Основная функция измерения   ********************************************************************
//****************************************************************************************************************************

void Measuring::timeOut() // функция срабатывающая по секундному таймеру (самая большая!!!)
{
    time+=1;    
    ui->label_4->setText(QString::number(time)); // отображение времени
    if(Time_threshold!=0&&time==Time_threshold) // срабатывания порога по времени
    {
        ui->label_4->setText("Порог - "+QString::number(Time_threshold)+"c");
        emit threshold_stop();
    }


//***********************------------------------------------------------------------*********************
//***********************--= Преобразование величины с АЦП в измеряемые параметры =--*********************
//**************************------------------------------------------------------************************

    ADC_V=compensation_ADC_V();  // функция учитывающая выполненную компенсацию по диапазонам (если не стоит галочка то не учитываем)

    //************************************************************************************************
    //********************     Измерение На чувствительном диапазоне       ***************************
    //************************************************************************************************

    if (measurement_range =="1")  //Чувствительный
    {        
        double x =ADC_V;
        double real_I = x/(R3*1000000000);          // реальные значение тока в экспонентной форме А
        integral_I+=real_I;                         // реальные значение интеграла тока в экспонентной форме Кл

        ui->label_24->setText(QString::number(real_I));  //отображение реального тока --Потом удалить--

         //************************ Измерение тока на чувствительном диапазоне *****************************************************
        if(measuring_parameter=="1") //Измерение тока
        {
            if(abs(real_I)<1e-9)
            {
                ui->label_8->setText("пА");  // отображение
                double z=0;
                z=real_I*1e12;

                displaingMes_label_7(z);

                if (abs(real_I)>=2e-10)
                {
                    connect(this,SIGNAL(changeRangeSIG()),this,SLOT(changeRangeSLOT()));
                    emit changeRangeSIG();
                    ui->label_23->setText("Смените диапазон");
                    ui->label_7->setText("ERROR ");
                }

                //********************** Интеграл тока на чувствительном диапазоне *****************************************

                ui->label_25->setText(QString::number(integral_I)); // ------удалить!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                if(abs(integral_I)<1e-9) // отображение заряда (интеграла тока) с размерностью
                {
                    ui->label_11->setText("пКл");
                    double z_integral_I = integral_I*1e12; // перевод в 4 цифры с точкой
                    displaingMes_label_10(z_integral_I);
                }
                if(abs(integral_I)>=1e-9 && integral_I<1e-6) // отображение заряда (интеграла тока) с размерностью
                {
                    ui->label_11->setText("нКл");
                    double z_integral_I = integral_I*1e9; // перевод в 4 цифры с точкой
                    displaingMes_label_10(z_integral_I);
                }
                if(abs(integral_I)>=1e-6 && integral_I<20e-6) // отображение заряда (интеграла тока) с размерностью
                {
                    ui->label_11->setText("мкКл");
                    double z_integral_I = integral_I*1e6; // перевод в 4 цифры с точкой
                    displaingMes_label_10(z_integral_I);
                }
                else
                    if(abs(integral_I)>=20e-6) ui->label_10->setText("ERROR"); //на чувствительном диапазоне макс 20 мкКл (100000 сек на макс токе 200 пА)
            }            
        }

        if(measuring_parameter=="2") //Измерение интеграла тока - функция обратная от тока (без СКО)
        {
            if(abs(real_I)<1e-9) // измерение тока - опускаем вниз
            {
                ui->label_11->setText("пА");  // отображение
                double z=0;
                z=real_I*1e12;
                displaingMes_label_10(z);

                if (abs(real_I)>=2e-10)
                {
                    connect(this,SIGNAL(changeRangeSIG()),this,SLOT(changeRangeSLOT()));
                    emit changeRangeSIG();
                    ui->label_23->setText("Смените диапазон");
                    ui->label_10->setText("ERROR ");
                }

                //********************** Интеграл тока на чувствительном диапазоне *****************************************
                if(abs(integral_I)<1e-9) // отображение заряда (интеграла тока) с размерностью
                {
                    ui->label_8->setText("пКл");
                    double z_integral_I = integral_I*1e12; // перевод в 4 цифры с точкой
                    displaingMes_label_7(z_integral_I);
                }
                if(abs(integral_I)>=1e-9 && integral_I<1e-6) // отображение заряда (интеграла тока) с размерностью
                {
                    ui->label_8->setText("нКл");
                    double z_integral_I = integral_I*1e9; // перевод в 4 цифры с точкой
                    displaingMes_label_7(z_integral_I);
                }
                if(abs(integral_I)>=1e-6 && integral_I<20e-6) // отображение заряда (интеграла тока) с размерностью
                {
                    ui->label_8->setText("мкКл");
                    double z_integral_I = integral_I*1e6; // перевод в 4 цифры с точкой
                    displaingMes_label_7(z_integral_I);
                }
                else
                    if(abs(integral_I)>=20e-6) ui->label_7->setText("ERROR"); //на чувствительном диапазоне макс 20 мкКл (100000 сек на макс токе 200 пА)
            }
        }

        //************************* Измерение заряда на чувствительном диапазоне ***************************************************

        if(measuring_parameter=="3") //Измерение Заряда
        {
            double x =ADC_V; //значение АЦП в В
            double real_Q1 = x*C1*(1e-12); // реальное значение заряда на конденсаторе в экспоненциальной форме
            ui->label_26->setText(QString::number(real_Q1));

            if(abs(real_Q1)<1.2e-9)                                   // до 1000 пКл
            {
                ui->label_8->setText("пКл");  // отображение
                ui->label_11->setText("пА");  // отображение
                double z = real_Q1*1e12;
                double q;
                q= z-Q;
                Q=z;
                if(time>=2)
                {
                D+=q;  // начинаем считать среднюю производную дозы со второго отсчета (на первом какую то ересь показывает)
                }
                double t=D/(time-1);
                displaingMes_label_7(z);
                displaingMes_label_10(t);

                //*********************Отображение тока*****************************
            }
            if (abs(real_Q1)>=9.99e-10)
            {
                connect(this,SIGNAL(changeRangeSIG()),this,SLOT(changeRangeSLOT()));
                emit changeRangeSIG();
                ui->label_23->setText("Смените диапазон");
                ui->label_7->setText("ERROR ");
            }
        }

        //*************************  измерение МАЭД (МПД, МЭД) на чувствительном диапазоне

        if(measuring_parameter=="4")     //измерение МАЭД (МПД, МЭД)
        {
            double doseRate = abs(real_I*correctionF*sensibility*760*(273+temperature)/(293*pressure));
            integral_DoseRate+=doseRate;
            if(DOSE_threshold!=0.&&integral_DoseRate>DOSE_threshold) // срабатывания порога по времени
            {
                emit threshold_stop();
                ui->label_4->setText("Достигнут порог по дозе за " + QString::number(time) + " c");
            }
            ui->label_26->setText(QString::number(doseRate)); // удалить!!!!!!!

            if(abs(real_I)<1e-9)
            {
                    if(measuring_parameter_dimension == "Гр/с" ||measuring_parameter_dimension == "Зв/с" ||measuring_parameter_dimension == "Р/с")
                    {
                        double z = doseRate*1e12; //  в сек
                        ui->label_25->setText(QString::number(z)); // удалить!!!!!!!
                        if (z<1e3)
                        {
                            ui->label_8->setText("п"+measuring_parameter_dimension);  // отображение
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e3 && z<1e6)
                        {
                            ui->label_8->setText("н"+measuring_parameter_dimension);  // отображение
                            z=z/1e3;
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e6 && z<1e9)
                        {
                            ui->label_8->setText("мк"+measuring_parameter_dimension);  // отображение
                            z=z/1e6;
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e9 && z<1e12)
                        {
                            ui->label_8->setText("м"+measuring_parameter_dimension);  // отображение
                            z=z/1e9;
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e12)
                        {
                            ui->label_8->setText(measuring_parameter_dimension);  // отображение
                            z=z/1e12;
                            displaingMes_label_7(z); // функция отображения
                        }

                    }
                    if(measuring_parameter_dimension == "Гр/мин" ||measuring_parameter_dimension == "Зв/мин" ||measuring_parameter_dimension == "Р/мин")
                    {
                        double z = doseRate*60*1e12; //  в сек
                        ui->label_25->setText(QString::number(z)); // удалить!!!!!!!
                        if (z<1e3)
                        {
                            ui->label_8->setText("п"+measuring_parameter_dimension);  // отображение
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e3 && z<1e6)
                        {
                            ui->label_8->setText("н"+measuring_parameter_dimension);  // отображение
                            z=z/1e3;
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e6 && z<1e9)
                        {
                            ui->label_8->setText("мк"+measuring_parameter_dimension);  // отображение
                            z=z/1e6;
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e9 && z<1e12)
                        {
                            ui->label_8->setText("м"+measuring_parameter_dimension);  // отображение
                            z=z/1e9;
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e12)
                        {
                            ui->label_8->setText(measuring_parameter_dimension);  // отображение
                            z=z/1e12;
                            displaingMes_label_7(z); // функция отображения
                        }
                    }
                    if(measuring_parameter_dimension == "Гр/ч" ||measuring_parameter_dimension == "Зв/ч" ||measuring_parameter_dimension == "Р/ч")
                    {
                        double z = doseRate*3600*1e12; //  в сек
                        ui->label_25->setText(QString::number(z)); // удалить!!!!!!!
                        if (z<1e3)
                        {
                            ui->label_8->setText("п"+measuring_parameter_dimension);  // отображение
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e3 && z<1e6)
                        {
                            ui->label_8->setText("н"+measuring_parameter_dimension);  // отображение
                            z=z/1e3;
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e6 && z<1e9)
                        {
                            ui->label_8->setText("мк"+measuring_parameter_dimension);  // отображение
                            z=z/1e6;
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e9 && z<1e12)
                        {
                            ui->label_8->setText("м"+measuring_parameter_dimension);  // отображение
                            z=z/1e9;
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e12)
                        {
                            ui->label_8->setText(measuring_parameter_dimension);  // отображение
                            z=z/1e12;
                            displaingMes_label_7(z); // функция отображения
                        }
                    }
            }
            if (abs(real_I)>=2e-10)
            {
                connect(this,SIGNAL(changeRangeSIG()),this,SLOT(changeRangeSLOT()));
                emit changeRangeSIG();
                ui->label_23->setText("Смените диапазон");
                ui->label_7->setText("ERROR ");
            }

        //********************************* интеграл МАЭД *************************************************************************        
            if(integral_DoseRate<1e-9)
            {
                ui->label_11->setText("п"+dimensionSensibility);
                double z=integral_DoseRate*1e12;
                displaingMes_label_10(z);
            }
            if(integral_DoseRate>=1e-9 && integral_DoseRate<1e-6)
            {
                ui->label_11->setText("н"+dimensionSensibility);
                double z=integral_DoseRate*1e9;
                displaingMes_label_10(z);
            }
            if(integral_DoseRate>=1e-6 && integral_DoseRate<1e-3)
            {
                ui->label_11->setText("мк"+dimensionSensibility);
                double z=integral_DoseRate*1e6;
                displaingMes_label_10(z);
            }
            if(integral_DoseRate>=1e-3 && integral_DoseRate<1e-0)
            {
                ui->label_11->setText("м"+dimensionSensibility);
                double z=integral_DoseRate*1e3;
                displaingMes_label_10(z);
            }
            if(integral_DoseRate>=1e0)
            {
                ui->label_11->setText(dimensionSensibility);
                double z=integral_DoseRate;
                displaingMes_label_10(z);
            }
        }

        if(measuring_parameter=="5")     //измерение интеграла МАЭД (МПД, МЭД)
        {
            double doseRate = abs(real_I*correctionF*sensibility*760*(273+temperature)/(293*pressure));
            integral_DoseRate+=doseRate;
            if(DOSE_threshold!=0.&&integral_DoseRate>DOSE_threshold) // срабатывания порога по времени
            {
                emit threshold_stop();
                ui->label_4->setText("Достигнут порог по дозе за " + QString::number(time) + " c");
            }
            if(abs(real_I)<1e-9)
            {
                    if(measuring_parameter_dimension == "Гр/с" ||measuring_parameter_dimension == "Зв/с" ||measuring_parameter_dimension == "Р/с")
                    {
                        double z = doseRate*1e12; //  в сек
                        ui->label_25->setText(QString::number(z)); // удалить!!!!!!!
                        if (z<1e3)
                        {
                            ui->label_11->setText("п"+measuring_parameter_dimension);  // отображение
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e3 && z<1e6)
                        {
                            ui->label_11->setText("н"+measuring_parameter_dimension);  // отображение
                            z=z/1e3;
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e6 && z<1e9)
                        {
                            ui->label_11->setText("мк"+measuring_parameter_dimension);  // отображение
                            z=z/1e6;
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e9 && z<1e12)
                        {
                            ui->label_11->setText("м"+measuring_parameter_dimension);  // отображение
                            z=z/1e9;
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e12)
                        {
                            ui->label_11->setText(measuring_parameter_dimension);  // отображение
                            z=z/1e12;
                            displaingMes_label_10(z); // функция отображения
                        }
                    }
                    if(measuring_parameter_dimension == "Гр/мин" ||measuring_parameter_dimension == "Зв/мин" ||measuring_parameter_dimension == "Р/мин")
                    {
                        double z = doseRate*60*1e12; //  в сек
                        ui->label_25->setText(QString::number(z)); // удалить!!!!!!!
                        if (z<1e3)
                        {
                            ui->label_11->setText("п"+measuring_parameter_dimension);  // отображение
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e3 && z<1e6)
                        {
                            ui->label_11->setText("н"+measuring_parameter_dimension);  // отображение
                            z=z/1e3;
                            displaingMes_label_7(10); // функция отображения
                        }
                        if (z>=1e6 && z<1e9)
                        {
                            ui->label_11->setText("мк"+measuring_parameter_dimension);  // отображение
                            z=z/1e6;
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e9 && z<1e12)
                        {
                            ui->label_11->setText("м"+measuring_parameter_dimension);  // отображение
                            z=z/1e9;
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e12)
                        {
                            ui->label_11->setText(measuring_parameter_dimension);  // отображение
                            z=z/1e12;
                            displaingMes_label_10(z); // функция отображения
                        }
                    }
                    if(measuring_parameter_dimension == "Гр/ч" ||measuring_parameter_dimension == "Зв/ч" ||measuring_parameter_dimension == "Р/ч")
                    {
                        double z = doseRate*3600*1e12; //  в сек
                        ui->label_25->setText(QString::number(z)); // удалить!!!!!!!
                        if (z<1e3)
                        {
                            ui->label_11->setText("п"+measuring_parameter_dimension);  // отображение
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e3 && z<1e6)
                        {
                            ui->label_11->setText("н"+measuring_parameter_dimension);  // отображение
                            z=z/1e3;
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e6 && z<1e9)
                        {
                            ui->label_11->setText("мк"+measuring_parameter_dimension);  // отображение
                            z=z/1e6;
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e9 && z<1e12)
                        {
                            ui->label_11->setText("м"+measuring_parameter_dimension);  // отображение
                            z=z/1e9;
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e12)
                        {
                            ui->label_11->setText(measuring_parameter_dimension);  // отображение
                            z=z/1e12;
                            displaingMes_label_10(z); // функция отображения
                        }
                    }
            }
            if (abs(real_I)>=2e-10)
            {
                connect(this,SIGNAL(changeRangeSIG()),this,SLOT(changeRangeSLOT()));
                emit changeRangeSIG();
                ui->label_23->setText("Смените диапазон");
                ui->label_7->setText("ERROR ");
            }
            //********************************* интеграл МАЭД *************************************************************************
            //qDebug()<<integral_DoseRate;
                if(integral_DoseRate<1e-9)
                {
                    ui->label_8->setText("п"+dimensionSensibility);
                    double z=integral_DoseRate*1e12;
                    displaingMes_label_7(z);
                }
                if(integral_DoseRate>=1e-9 && integral_DoseRate<1e-6)
                {
                    ui->label_8->setText("н"+dimensionSensibility);
                    double z=integral_DoseRate*1e9;
                    displaingMes_label_7(z);
                }
                if(integral_DoseRate>=1e-6 && integral_DoseRate<1e-3)
                {
                    ui->label_8->setText("мк"+dimensionSensibility);
                    double z=integral_DoseRate*1e6;
                    displaingMes_label_7(z);
                }
                if(integral_DoseRate>=1e-3 && integral_DoseRate<1e-0)
                {
                    ui->label_8->setText("м"+dimensionSensibility);
                    double z=integral_DoseRate*1e3;
                    displaingMes_label_7(z);
                }
                if(integral_DoseRate>=1e0)
                {
                    ui->label_8->setText(dimensionSensibility);
                    double z=integral_DoseRate;
                    displaingMes_label_7(z);
                }
        }

        if(measuring_parameter=="6")     //измерение дозы на конденсаторе на чувствительном диапазоне
        {
            double ADC = ADC_V; //значение АЦП в В
            double real_Q1 = ADC*C1*(1e-12); // реальное значение заряда на конденсаторе в экспоненциальной форме
            double realDose = abs(real_Q1*correctionF*sensibility*760*(273+temperature)/(293*pressure));
            if(DOSE_threshold!=0.&&realDose>DOSE_threshold) // срабатывания порога по времени
            {
                emit threshold_stop();
                ui->label_4->setText("Достигнут порог по дозе за " + QString::number(time) + " c");
            }
            if(abs(real_Q1)<1.2e-9)                                   // до 1000 пКл
            {
                if(realDose<1e-9)
                {
                    ui->label_8->setText("п"+dimensionSensibility);  // отображение
                    double z = realDose*1e12;
                    displaingMes_label_7(z);
                }
                if(realDose>=1e-9 && realDose<1e-6)
                {
                    ui->label_8->setText("н"+dimensionSensibility);  // отображение
                    double z = realDose*1e9;
                    displaingMes_label_7(z);
                }
                if(realDose>=1e-6 && realDose<1e-3)
                {
                    ui->label_8->setText("мк"+dimensionSensibility);  // отображение
                    double z = realDose*1e6;
                    displaingMes_label_7(z);
                }
                if(realDose>=1e-3 && realDose<1e0)
                {
                    ui->label_8->setText("м"+dimensionSensibility);  // отображение
                    double z = realDose*1e3;
                   displaingMes_label_7(z);
                }

                double DOSE = realDose-D; //
                D=realDose;
                if(time>=2)
                {
                Q+=DOSE;  // начинаем считать среднюю производную дозы со второго отсчета (на первом какую то ересь показывает)
                }
                if(measuring_parameter_dimension == "Гр/с" ||measuring_parameter_dimension == "Зв/с" ||measuring_parameter_dimension == "Р/с")
                {
                    double DOSE1=abs(Q/(time-1));
                    if(DOSE1<1e-9)
                    {
                        ui->label_11->setText("п"+dimensionSensibility+"/c");  // отображение
                        double z = DOSE1*1e12;
                        displaingMes_label_10(z);
                    }
                    if(DOSE1>=1e-9 && DOSE1<1e-6)
                    {
                        ui->label_11->setText("н"+dimensionSensibility+"/c");  // отображение
                        double z = DOSE1*1e9;
                        displaingMes_label_10(z);
                    }
                    if(DOSE1>=1e-6 && DOSE1<1e-3)
                    {
                        ui->label_11->setText("мк"+dimensionSensibility+"/c");  // отображение
                        double z = DOSE1*1e6;
                        displaingMes_label_10(z);
                    }
                    if(DOSE1>=1e-3 && DOSE1<1e0)
                    {
                        ui->label_11->setText("м"+dimensionSensibility+"/c");  // отображение
                        double z = DOSE1*1e3;
                       displaingMes_label_10(z);
                    }
                }
                if(measuring_parameter_dimension == "Гр/мин" ||measuring_parameter_dimension == "Зв/мин" ||measuring_parameter_dimension == "Р/мин")
                {
                    double DOSE1=abs(Q*60/(time-1));
                    if(DOSE1<1e-9)
                    {
                        ui->label_11->setText("п"+dimensionSensibility+"/мин");  // отображение
                        double z = DOSE1*1e12;
                        displaingMes_label_10(z);
                    }
                    if(DOSE1>=1e-9 && DOSE1<1e-6)
                    {
                        ui->label_11->setText("н"+dimensionSensibility+"/мин");  // отображение
                        double z = DOSE1*1e9;
                        displaingMes_label_10(z);
                    }
                    if(DOSE1>=1e-6 && DOSE1<1e-3)
                    {
                        ui->label_11->setText("мк"+dimensionSensibility+"/мин");  // отображение
                        double z = DOSE1*1e6;
                        displaingMes_label_10(z);
                    }
                    if(DOSE1>=1e-3 && DOSE1<1e0)
                    {
                        ui->label_11->setText("м"+dimensionSensibility+"/мин");  // отображение
                        double z = DOSE1*1e3;
                       displaingMes_label_10(z);
                    }
                }
                if(measuring_parameter_dimension == "Гр/ч" ||measuring_parameter_dimension == "Зв/ч" ||measuring_parameter_dimension == "Р/ч")
                {
                    double DOSE1=abs(Q*3600/(time-1));
                    if(DOSE1<1e-9)
                    {
                        ui->label_11->setText("п"+dimensionSensibility+"/ч");  // отображение
                        double z = DOSE1*1e12;
                        displaingMes_label_10(z);
                    }
                    if(DOSE1>=1e-9 && DOSE1<1e-6)
                    {
                        ui->label_11->setText("н"+dimensionSensibility+"/ч");  // отображение
                        double z = DOSE1*1e9;
                        displaingMes_label_10(z);
                    }
                    if(DOSE1>=1e-6 && DOSE1<1e-3)
                    {
                        ui->label_11->setText("мк"+dimensionSensibility+"/ч");  // отображение
                        double z = DOSE1*1e6;
                        displaingMes_label_10(z);
                    }
                    if(DOSE1>=1e-3 && DOSE1<1e0)
                    {
                        ui->label_11->setText("м"+dimensionSensibility+"/ч");  // отображение
                        double z = DOSE1*1e3;
                       displaingMes_label_10(z);
                    }
                    if(DOSE1>1e0)
                    {
                        ui->label_11->setText(""+dimensionSensibility+"/ч");  // отображение
                        double z = DOSE1;
                       displaingMes_label_10(z);
                    }
                }
            }
            if (abs(real_Q1)>=9.99e-10)  // если больше 1000 пКл - сообщение об ошибке
            {
                connect(this,SIGNAL(changeRangeSIG()),this,SLOT(changeRangeSLOT()));
                emit changeRangeSIG();
                ui->label_23->setText("Смените диапазон");
                ui->label_7->setText("ERROR ");
            }
        }
    }

    //****************************************************************************************************************************
    //********************     Измерение На среднем диапазоне       **************************************************************
    //****************************************************************************************************************************

    if (measurement_range =="2") // Средний
    {
        double x =ADC_V;
        double real_I = x/(R2*1000000);
        integral_I+=real_I;
        ui->label_24->setText(QString::number(real_I));  //отображение реального тока --Потом удалить--

        //************************ Измерение тока на среднем диапазоне *****************************************************
        if(measuring_parameter=="1")
        {
            if(abs(real_I)<2e-10)
            {
                ui->label_23->setText("Низкая точность - смените диапазон");
            }else{ui->label_23->setText("");}
            if(abs(real_I)<1e-9)
            {
                ui->label_8->setText("пА");
                double z=0;
                z=real_I*1e12;
                displaingMes_label_7(z);                
            }
            if(abs(real_I)>=1e-9 && real_I<1e-6)
            {
                ui->label_8->setText("нА");
                double z=0;
                z=real_I*1e9;
                displaingMes_label_7(z);
                if (real_I>=2e-8)
                {
                    connect(this,SIGNAL(changeRangeSIG()),this,SLOT(changeRangeSLOT()));
                    emit changeRangeSIG();
                    ui->label_23->setText("Смените диапазон");
                    ui->label_7->setText("ERROR ");
                }
            }

            //********************** Интеграл тока на среднем диапазоне *****************************************
            if(abs(integral_I)<1e-9) // отображение заряда (интеграла тока) с размерностью
            {
                ui->label_11->setText("пКл");
                double z_integral_I = integral_I*1e12; // перевод в 4 цифры с точкой
                displaingMes_label_10(z_integral_I);
            }
            if(abs(integral_I)>=1e-9 && abs(integral_I)<1e-6) // отображение заряда (интеграла тока) с размерностью
            {
                ui->label_11->setText("нКл");
                double z_integral_I = integral_I*1e9; // перевод в 4 цифры с точкой
                displaingMes_label_10(z_integral_I);
            }
            if(abs(integral_I)>=1e-6 && abs(integral_I)<1e-3) // отображение заряда (интеграла тока) с размерностью
            {
                ui->label_11->setText("мкКл");
                double z_integral_I = integral_I*1e6; // перевод в 4 цифры с точкой
                displaingMes_label_10(z_integral_I);
            }
            if(abs(integral_I)>1e-3 && abs(integral_I)<2e-3) // отображение заряда (интеграла тока) с размерностью
            {
                ui->label_11->setText("мКл");
                double z_integral_I = integral_I*1e3; // перевод в 4 цифры с точкой
                displaingMes_label_10(z_integral_I);
            }
            else
                if(abs(integral_I)>=2e-3) ui->label_10->setText("ERROR"); //на среднем диапазоне макс 2 мКл (100000 сек на макс токе 20 нА)
        }

        if(measuring_parameter=="2") //Измерение интеграла тока - функция обратная от тока (без СКО)
        {
            if(abs(real_I)<2e-10)
            {
                ui->label_23->setText("Низкая точность - смените диапазон");
            }else{ui->label_23->setText("");}
            if(abs(real_I)<1e-9)
            {
                ui->label_11->setText("пА");
                double z=0;
                z=real_I*1e12;
                displaingMes_label_7(z);
                if (abs(real_I)>=2e-10)
                {
                    connect(this,SIGNAL(changeRangeSIG()),this,SLOT(changeRangeSLOT()));
                    emit changeRangeSIG();
                    ui->label_23->setText("Смените диапазон");
                    ui->label_10->setText("ERROR ");
                }
            }
            if(abs(real_I)>=1e-9 && abs(real_I)<1e-6)
            {
                ui->label_11->setText("нА");
                double z=0;
                z=real_I*1e9;
                displaingMes_label_7(z);
                if (abs(real_I)>=2e-8)
                {
                    connect(this,SIGNAL(changeRangeSIG()),this,SLOT(changeRangeSLOT()));
                    emit changeRangeSIG();
                    ui->label_23->setText("Смените диапазон");
                    ui->label_10->setText("ERROR ");
                }
            }

            //********************** Интеграл тока на среднем диапазоне *****************************************

            ui->label_25->setText(QString::number(integral_I)); // ------удалить!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            if(abs(integral_I)<1e-9) // отображение заряда (интеграла тока) с размерностью
            {
                ui->label_8->setText("пКл");
                double z_integral_I = integral_I*1e12; // перевод в 4 цифры с точкой
                if(QString::number(z_integral_I,'f',3).size()<6) // пКл от 0.000 до 9.999
                {
                    ui->label_7->setText(QString::number(z_integral_I,'f',3));
                }
                else
                    if(QString::number(z_integral_I,'f',3).size()>=6 && QString::number(z_integral_I,'f',3).size()<7)
                    {
                        ui->label_7->setText(QString::number(z_integral_I,'f',2));// пКл от 10.00 до 99.99
                    }
                    else
                        if(QString::number(z_integral_I,'f',3).size()>=7 && QString::number(z_integral_I,'f',3).size()<8)
                        {
                            ui->label_7->setText(QString::number(z_integral_I,'f',1));// пКл от 100.0 до 999.9
                        }
            }
            if(abs(integral_I)>=1e-9 && abs(integral_I)<1e-6) // отображение заряда (интеграла тока) с размерностью
            {
                ui->label_8->setText("нКл");
                double z_integral_I = integral_I*1e9; // перевод в 4 цифры с точкой
                displaingMes_label_7(z_integral_I);
            }
            if(abs(integral_I)>=1e-6 && abs(integral_I)<1e-3) // отображение заряда (интеграла тока) с размерностью
            {
                ui->label_8->setText("мкКл");
                double z_integral_I = integral_I*1e6; // перевод в 4 цифры с точкой
                displaingMes_label_7(z_integral_I);
            }
            if(abs(integral_I)>1e-3 && abs(integral_I)<2e-3) // отображение заряда (интеграла тока) с размерностью
            {
                ui->label_8->setText("мКл");
                double z_integral_I = integral_I*1e3; // перевод в 4 цифры с точкой
                displaingMes_label_7(z_integral_I);
            }
            else
                if(abs(integral_I)>=2e-3) ui->label_7->setText("ERROR"); //на среднем диапазоне макс 2 мКл (100000 сек на макс токе 20 нА)
        }

        //************************* Измерение заряда на среднем и грубом диапазоне ***************************************************

        if(measuring_parameter=="3") //Измерение Заряда
        {
            double x = ADC_V; //значение АЦП в В
            double real_Q1 = x*C2*(1e-12); // реальное значение заряда на конденсаторе в экспоненциальной форме            
            if(abs(real_Q1)<1e-9)
            {
                ui->label_23->setText("Низкая точность - смените диапазон");
            }else{ui->label_23->setText("");}
            if(abs(real_Q1)<1e-9)                                   // до 1000 пКл
            {
                ui->label_8->setText("пКл");  // отображение
                double z = real_Q1*1e12;
                double q;
                q= real_Q1-Q;
                Q=real_Q1;
                if(time>=2)
                {
                D+=q;  // начинаем считать среднюю производную дозы со второго отсчета (на первом какую то ересь показывает)
                }
                double t=D/(time-1);
                displaingMes_label_7(z);

                if(t<1e-9)
                {
                    ui->label_11->setText("пА");  // отображение
                    double t1=t*1e12;
                    displaingMes_label_10(t1);
                }
                if(t>=1e-9 && t<1e-6)
                {
                    ui->label_11->setText("нА");  // отображение
                    double t1=t*1e9;
                    displaingMes_label_10(t1);
                }
            }
            if(abs(real_Q1)>=1e-9 && abs(real_Q1)<100.2e-9)                                   // до 100 нКл
            {
                ui->label_8->setText("нКл");  // отображение
                double z = real_Q1*1e9;
                double q;
                q= real_Q1-Q;
                Q=real_Q1;
                if(time>=2)
                {
                D+=q;  // начинаем считать среднюю производную дозы со второго отсчета (на первом какую то ересь показывает)
                }
                double t=D/(time-1);
                displaingMes_label_7(z);
                if(t<1e-9)
                {
                    ui->label_11->setText("пА");  // отображение
                    double t1=t*1e12;
                    displaingMes_label_10(t1);
                }
                if(t>=1e-9 && t<1e-6)
                {
                    ui->label_11->setText("нА");  // отображение
                    double t1=t*1e9;
                    displaingMes_label_10(t1);
                }
            }
            if (abs(real_Q1)>=99.9e-9) // если больше 100 нКл - сообщение об ошибке
            {
                connect(this,SIGNAL(changeRangeSIG()),this,SLOT(changeRangeSLOT()));
                emit changeRangeSIG();
                ui->label_23->setText("Смените камеру");
                ui->label_7->setText("ERROR ");
            }
        }

        if(measuring_parameter=="4")     //измерение МАЭД (МПД, МЭД)
        {
            double doseRate = abs(real_I*correctionF*sensibility*760*(273+temperature)/(293*pressure));
            integral_DoseRate+=doseRate;
            if(DOSE_threshold!=0.&&integral_DoseRate>DOSE_threshold) // срабатывания порога по времени
            {
                emit threshold_stop();
                ui->label_4->setText("Достигнут порог по дозе за " + QString::number(time) + " c");
            }
            if(abs(real_I)<2e-10)
            {
                ui->label_23->setText("Низкая точность - смените диапазон");
            }else{ui->label_23->setText("");}
            if(abs(real_I)<100e-9)
            {
                    if(measuring_parameter_dimension == "Гр/с" ||measuring_parameter_dimension == "Зв/с" ||measuring_parameter_dimension == "Р/с")
                    {
                        double z = doseRate*1e12; //  в сек
                        ui->label_25->setText(QString::number(z)); // удалить!!!!!!!
                        if (z<1e3)
                        {
                            ui->label_8->setText("п"+measuring_parameter_dimension);  // отображение
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e3 && z<1e6)
                        {
                            ui->label_8->setText("н"+measuring_parameter_dimension);  // отображение
                            z=z/1e3;
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e6 && z<1e9)
                        {
                            ui->label_8->setText("мк"+measuring_parameter_dimension);  // отображение
                            z=z/1e6;
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e9 && z<1e12)
                        {
                            ui->label_8->setText("м"+measuring_parameter_dimension);  // отображение
                            z=z/1e9;
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e12)
                        {
                            ui->label_8->setText(measuring_parameter_dimension);  // отображение
                            z=z/1e12;
                            displaingMes_label_7(z); // функция отображения
                        }
                    }
                    if(measuring_parameter_dimension == "Гр/мин" ||measuring_parameter_dimension == "Зв/мин" ||measuring_parameter_dimension == "Р/мин")
                    {
                        double z = doseRate*60*1e12; //  в сек
                        ui->label_25->setText(QString::number(z)); // удалить!!!!!!!
                        if (z<1e3)
                        {
                            ui->label_8->setText("п"+measuring_parameter_dimension);  // отображение
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e3 && z<1e6)
                        {
                            ui->label_8->setText("н"+measuring_parameter_dimension);  // отображение
                            z=z/1e3;
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e6 && z<1e9)
                        {
                            ui->label_8->setText("мк"+measuring_parameter_dimension);  // отображение
                            z=z/1e6;
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e9 && z<1e12)
                        {
                            ui->label_8->setText("м"+measuring_parameter_dimension);  // отображение
                            z=z/1e9;
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e12)
                        {
                            ui->label_8->setText(measuring_parameter_dimension);  // отображение
                            z=z/1e12;
                            displaingMes_label_7(z); // функция отображения
                        }
                    }
                    if(measuring_parameter_dimension == "Гр/ч" ||measuring_parameter_dimension == "Зв/ч" ||measuring_parameter_dimension == "Р/ч")
                    {
                        double z = doseRate*3600*1e12; //  в сек
                        ui->label_25->setText(QString::number(z)); // удалить!!!!!!!
                        if (z<1e3)
                        {
                            ui->label_8->setText("п"+measuring_parameter_dimension);  // отображение
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e3 && z<1e6)
                        {
                            ui->label_8->setText("н"+measuring_parameter_dimension);  // отображение
                            z=z/1e3;
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e6 && z<1e9)
                        {
                            ui->label_8->setText("мк"+measuring_parameter_dimension);  // отображение
                            z=z/1e6;
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e9 && z<1e12)
                        {
                            ui->label_8->setText("м"+measuring_parameter_dimension);  // отображение
                            z=z/1e9;
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e12)
                        {
                            ui->label_8->setText(measuring_parameter_dimension);  // отображение
                            z=z/1e12;
                            displaingMes_label_7(z); // функция отображения
                        }
                    }
            }
            if (abs(real_I)>=20e-9)
            {
                connect(this,SIGNAL(changeRangeSIG()),this,SLOT(changeRangeSLOT()));
                emit changeRangeSIG();
                ui->label_23->setText("Смените диапазон");
                ui->label_7->setText("ERROR ");
            }

            //********************************* интеграл МАЭД *************************************************************************
            //qDebug()<<integral_DoseRate;
                if(integral_DoseRate<1e-9)
                {
                    ui->label_11->setText("п"+dimensionSensibility);
                    double z=integral_DoseRate*1e12;
                    displaingMes_label_10(z);
                }
                if(integral_DoseRate>=1e-9 && integral_DoseRate<1e-6)
                {
                    ui->label_11->setText("н"+dimensionSensibility);
                    double z=integral_DoseRate*1e9;
                    displaingMes_label_10(z);
                }
                if(integral_DoseRate>=1e-6 && integral_DoseRate<1e-3)
                {
                    ui->label_11->setText("мк"+dimensionSensibility);
                    double z=integral_DoseRate*1e6;
                    displaingMes_label_10(z);
                }
                if(integral_DoseRate>=1e-3 && integral_DoseRate<1e-0)
                {
                    ui->label_11->setText("м"+dimensionSensibility);
                    double z=integral_DoseRate*1e3;
                    displaingMes_label_10(z);
                }
                if(integral_DoseRate>=1e0)
                {
                    ui->label_11->setText(dimensionSensibility);
                    double z=integral_DoseRate;
                    displaingMes_label_10(z);
                }
        }

        if(measuring_parameter=="5")     //измерение интеграла МАЭД (МПД, МЭД)
        {
            double doseRate = abs(real_I*correctionF*sensibility*760*(273+temperature)/(293*pressure));
            integral_DoseRate+=doseRate;
            if(DOSE_threshold!=0.&&integral_DoseRate>DOSE_threshold) // срабатывания порога по времени
            {
                emit threshold_stop();
                ui->label_4->setText("Достигнут порог по дозе за " + QString::number(time) + " c");
            }
            if(abs(real_I)<21e-9)
            {
                if(abs(real_I)<2e-10)
                {
                    ui->label_23->setText("Низкая точность - смените диапазон");
                }else{ui->label_23->setText("");}

                    if(measuring_parameter_dimension == "Гр/с" ||measuring_parameter_dimension == "Зв/с" ||measuring_parameter_dimension == "Р/с")
                    {
                        double z = doseRate*1e12; //  в сек
                        ui->label_25->setText(QString::number(z)); // удалить!!!!!!!
                        if (z<1e3)
                        {
                            ui->label_11->setText("п"+measuring_parameter_dimension);  // отображение
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e3 && z<1e6)
                        {
                            ui->label_11->setText("н"+measuring_parameter_dimension);  // отображение
                            z=z/1e3;
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e6 && z<1e9)
                        {
                            ui->label_11->setText("мк"+measuring_parameter_dimension);  // отображение
                            z=z/1e6;
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e9 && z<1e12)
                        {
                            ui->label_11->setText("м"+measuring_parameter_dimension);  // отображение
                            z=z/1e9;
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e12)
                        {
                            ui->label_11->setText(measuring_parameter_dimension);  // отображение
                            z=z/1e12;
                            displaingMes_label_10(z); // функция отображения
                        }
                    }
                    if(measuring_parameter_dimension == "Гр/мин" ||measuring_parameter_dimension == "Зв/мин" ||measuring_parameter_dimension == "Р/мин")
                    {
                        double z = doseRate*60*1e12; //  в сек
                        ui->label_25->setText(QString::number(z)); // удалить!!!!!!!
                        if (z<1e3)
                        {
                            ui->label_11->setText("п"+measuring_parameter_dimension);  // отображение
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e3 && z<1e6)
                        {
                            ui->label_11->setText("н"+measuring_parameter_dimension);  // отображение
                            z=z/1e3;
                            displaingMes_label_7(10); // функция отображения
                        }
                        if (z>=1e6 && z<1e9)
                        {
                            ui->label_11->setText("мк"+measuring_parameter_dimension);  // отображение
                            z=z/1e6;
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e9 && z<1e12)
                        {
                            ui->label_11->setText("м"+measuring_parameter_dimension);  // отображение
                            z=z/1e9;
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e12)
                        {
                            ui->label_11->setText(measuring_parameter_dimension);  // отображение
                            z=z/1e12;
                            displaingMes_label_10(z); // функция отображения
                        }
                    }
                    if(measuring_parameter_dimension == "Гр/ч" ||measuring_parameter_dimension == "Зв/ч" ||measuring_parameter_dimension == "Р/ч")
                    {
                        double z = doseRate*3600*1e12; //  в сек
                        ui->label_25->setText(QString::number(z)); // удалить!!!!!!!
                        if (z<1e3)
                        {
                            ui->label_11->setText("п"+measuring_parameter_dimension);  // отображение
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e3 && z<1e6)
                        {
                            ui->label_11->setText("н"+measuring_parameter_dimension);  // отображение
                            z=z/1e3;
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e6 && z<1e9)
                        {
                            ui->label_11->setText("мк"+measuring_parameter_dimension);  // отображение
                            z=z/1e6;
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e9 && z<1e12)
                        {
                            ui->label_11->setText("м"+measuring_parameter_dimension);  // отображение
                            z=z/1e9;
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e12)
                        {
                            ui->label_11->setText(measuring_parameter_dimension);  // отображение
                            z=z/1e12;
                            displaingMes_label_10(z); // функция отображения
                        }
                    }
            }
            if (abs(real_I)>=20e-9)
            {
                connect(this,SIGNAL(changeRangeSIG()),this,SLOT(changeRangeSLOT()));
                emit changeRangeSIG();
                ui->label_23->setText("Смените диапазон");
                ui->label_7->setText("ERROR ");
            }
            //********************************* интеграл МАЭД *************************************************************************
            //qDebug()<<integral_DoseRate;
                if(integral_DoseRate<1e-9)
                {
                    ui->label_8->setText("п"+dimensionSensibility);
                    double z=integral_DoseRate*1e12;
                    displaingMes_label_7(z);
                }
                if(integral_DoseRate>=1e-9 && integral_DoseRate<1e-6)
                {
                    ui->label_8->setText("н"+dimensionSensibility);
                    double z=integral_DoseRate*1e9;
                    displaingMes_label_7(z);
                }
                if(integral_DoseRate>=1e-6 && integral_DoseRate<1e-3)
                {
                    ui->label_8->setText("мк"+dimensionSensibility);
                    double z=integral_DoseRate*1e6;
                    displaingMes_label_7(z);
                }
                if(integral_DoseRate>=1e-3 && integral_DoseRate<1e-0)
                {
                    ui->label_8->setText("м"+dimensionSensibility);
                    double z=integral_DoseRate*1e3;
                    displaingMes_label_7(z);
                }
                if(integral_DoseRate>=1e0)
                {
                    ui->label_8->setText(dimensionSensibility);
                    double z=integral_DoseRate;
                    displaingMes_label_7(z);
                }
        }

        if(measuring_parameter=="6")     //измерение дозы на конденсаторе на среднем и грубом диапазоне
        {
            double x = ADC_V; //значение АЦП в В
            double real_Q1 = x*C2*(1e-12); // реальное значение заряда на конденсаторе в экспоненциальной форме
            double realDose = abs(real_Q1*correctionF*sensibility*760*(273+temperature)/(293*pressure));
            if(DOSE_threshold!=0.&&realDose>DOSE_threshold) // срабатывания порога по времени
            {
                emit threshold_stop();
                ui->label_4->setText("Достигнут порог по дозе за " + QString::number(time) + " c");
            }
            if(abs(real_Q1)<1e-9)
            {
                ui->label_23->setText("Низкая точность - смените диапазон");
            }else{ui->label_23->setText("");}
            if(abs(real_Q1)<100.2e-9)                                   // до 100 нКл
            {                
                if(realDose<1e-6)
                {
                    ui->label_8->setText("н"+dimensionSensibility);  // отображение
                    double z = realDose*1e9;
                    displaingMes_label_7(z);
                }
                if(realDose>=1e-6 && realDose<1e-3)
                {
                    ui->label_8->setText("мк"+dimensionSensibility);  // отображение
                    double z = realDose*1e6;
                    displaingMes_label_7(z);
                }
                if(realDose>=1e-3 && realDose<1e0)
                {
                    ui->label_8->setText("м"+dimensionSensibility);  // отображение
                    double z = realDose*1e3;
                    displaingMes_label_7(z);
                }
                if(realDose>=1e0 && realDose<1e3)
                {
                    ui->label_8->setText(""+dimensionSensibility);  // отображение
                    double z = realDose*1e0;
                    displaingMes_label_7(z);
                }
                double DOSE = realDose-D; //
                D=realDose;
                if(time>=2)
                {
                Q+=DOSE;  // начинаем считать среднюю производную дозы со второго отсчета (на первом какую то ересь показывает)
                }
                if(measuring_parameter_dimension == "Гр/с" ||measuring_parameter_dimension == "Зв/с" ||measuring_parameter_dimension == "Р/с")
                {
                    double DOSE1=abs(Q/(time-1));
                    if(DOSE1<1e-9)
                    {
                        ui->label_11->setText("п"+dimensionSensibility+"/c");  // отображение
                        double z = DOSE1*1e12;
                        displaingMes_label_10(z);
                    }
                    if(DOSE1>=1e-9 && DOSE1<1e-6)
                    {
                        ui->label_11->setText("н"+dimensionSensibility+"/c");  // отображение
                        double z = DOSE1*1e9;
                        displaingMes_label_10(z);
                    }
                    if(DOSE1>=1e-6 && DOSE1<1e-3)
                    {
                        ui->label_11->setText("мк"+dimensionSensibility+"/c");  // отображение
                        double z = DOSE1*1e6;
                        displaingMes_label_10(z);
                    }
                    if(DOSE1>=1e-3 && DOSE1<1e0)
                    {
                        ui->label_11->setText("м"+dimensionSensibility+"/c");  // отображение
                        double z = DOSE1*1e3;
                       displaingMes_label_10(z);
                    }
                }
                if(measuring_parameter_dimension == "Гр/мин" ||measuring_parameter_dimension == "Зв/мин" ||measuring_parameter_dimension == "Р/мин")
                {
                    double DOSE1=abs(Q*60/(time-1));
                    if(DOSE1<1e-9)
                    {
                        ui->label_11->setText("п"+dimensionSensibility+"/мин");  // отображение
                        double z = DOSE1*1e12;
                        displaingMes_label_10(z);
                    }
                    if(DOSE1>=1e-9 && DOSE1<1e-6)
                    {
                        ui->label_11->setText("н"+dimensionSensibility+"/мин");  // отображение
                        double z = DOSE1*1e9;
                        displaingMes_label_10(z);
                    }
                    if(DOSE1>=1e-6 && DOSE1<1e-3)
                    {
                        ui->label_11->setText("мк"+dimensionSensibility+"/мин");  // отображение
                        double z = DOSE1*1e6;
                        displaingMes_label_10(z);
                    }
                    if(DOSE1>=1e-3 && DOSE1<1e0)
                    {
                        ui->label_11->setText("м"+dimensionSensibility+"/мин");  // отображение
                        double z = DOSE1*1e3;
                       displaingMes_label_10(z);
                    }
                    if(DOSE1>1e0)
                    {
                        ui->label_11->setText(""+dimensionSensibility+"/мин");  // отображение
                        double z = DOSE1;
                       displaingMes_label_10(z);
                    }
                }
                if(measuring_parameter_dimension == "Гр/ч" ||measuring_parameter_dimension == "Зв/ч" ||measuring_parameter_dimension == "Р/ч")
                {
                    double DOSE1=abs(Q*3600/(time-1));
                    if(DOSE1<1e-9)
                    {
                        ui->label_11->setText("п"+dimensionSensibility+"/ч");  // отображение
                        double z = DOSE1*1e12;
                        displaingMes_label_10(z);
                    }
                    if(DOSE1>=1e-9 && DOSE1<1e-6)
                    {
                        ui->label_11->setText("н"+dimensionSensibility+"/ч");  // отображение
                        double z = DOSE1*1e9;
                        displaingMes_label_10(z);
                    }
                    if(DOSE1>=1e-6 && DOSE1<1e-3)
                    {
                        ui->label_11->setText("мк"+dimensionSensibility+"/ч");  // отображение
                        double z = DOSE1*1e6;
                        displaingMes_label_10(z);
                    }
                    if(DOSE1>=1e-3 && DOSE1<1e0)
                    {
                        ui->label_11->setText("м"+dimensionSensibility+"/ч");  // отображение
                        double z = DOSE1*1e3;
                       displaingMes_label_10(z);
                    }
                    if(DOSE1>1e0)
                    {
                        ui->label_11->setText(""+dimensionSensibility+"/ч");  // отображение
                        double z = DOSE1;
                       displaingMes_label_10(z);
                    }
                }
            }
            if (abs(real_Q1)>=99.99e-9)  // если больше 100 нКл - сообщение об ошибке
            {
                connect(this,SIGNAL(changeRangeSIG()),this,SLOT(changeRangeSLOT()));
                emit changeRangeSIG();
                ui->label_23->setText("Смените диапазон");
                ui->label_7->setText("ERROR ");
            }
        }
    }

    //***********************************************************************************************************
    //********************     Измерение На грубом диапазоне       **********************************************
    //***********************************************************************************************************

    if (measurement_range =="3") // Грубый
    {
        double x =ADC_V;
        double real_I = x/(R1*1000000);
        integral_I+=real_I;
        ui->label_24->setText(QString::number(real_I));  //отображение реального тока --Потом удалить--

         //************************ Измерение тока на грубом диапазоне *****************************************************

        if(measuring_parameter=="1")
        {
            if(abs(real_I)<2e-8)
            {
                ui->label_23->setText("Низкая точность - смените диапазон");
            }else{ui->label_23->setText("");}
            if (abs(real_I)<1e-9)
            {
                ui->label_8->setText("пА");
                double z=0;
                z=real_I*1e12;
                displaingMes_label_7(z);
            }
            if(abs(real_I)>=1e-9 && abs(real_I)<1e-6)
            {
                ui->label_8->setText("нА");
                double z=0;
                z=real_I*1e9;
                displaingMes_label_7(z);
            }
            if(abs(real_I)>=1e-6)
            {
                ui->label_8->setText("мкА");
                double z=0;
                z=real_I*1e6;
                displaingMes_label_7(z);
                if (abs(real_I)>=2e-6)
                {
                    connect(this,SIGNAL(changeRangeSIG()),this,SLOT(changeRangeSLOT()));
                    emit changeRangeSIG();
                    ui->label_23->setText("Смените камеру");
                    ui->label_7->setText("ERROR ");
                }
            }

            //********************** Интеграл тока на грубом диапазоне *****************************************
            if(abs(integral_I)<1e-9) // отображение заряда (интеграла тока) с размерностью
            {
                ui->label_11->setText("пКл");
                double z_integral_I = integral_I*1e12; // перевод в 4 цифры с точкой
                displaingMes_label_10(z_integral_I);
            }
            if(abs(integral_I)>=1e-9 && abs(integral_I)<1e-6) // отображение заряда (интеграла тока) с размерностью
            {
                ui->label_11->setText("нКл");
                double z_integral_I = integral_I*1e9; // перевод в 4 цифры с точкой
                displaingMes_label_10(z_integral_I);
            }
            if(abs(integral_I)>=1e-6 && abs(integral_I)<1e-3) // отображение заряда (интеграла тока) с размерностью
            {
                ui->label_11->setText("мкКл");
                double z_integral_I = integral_I*1e6; // перевод в 4 цифры с точкой
                displaingMes_label_10(z_integral_I);
            }
            if(abs(integral_I)>1e-3 && abs(integral_I)<200e-3) // отображение заряда (интеграла тока) с размерностью
            {
                ui->label_11->setText("мКл");
                double z_integral_I = integral_I*1e3; // перевод в 4 цифры с точкой
                displaingMes_label_10(z_integral_I);
            }
            else
                if(abs(integral_I)>=200e-3) ui->label_10->setText("ERROR"); //на грубом диапазоне макс 200 мКл (100000 сек на макс токе 2 мкА)
        }

        if(measuring_parameter=="2") // Измерение интеграла тока на грубом диапазоне
        {
            if(abs(real_I)<2e-8)
            {
                ui->label_23->setText("Низкая точность - смените диапазон");
            }else{ui->label_23->setText("");}
            if (abs(real_I)<1e-9)
            {
                ui->label_11->setText("пА");
                double z=0;
                z=real_I*1e12;
                displaingMes_label_10(z);
            }
            if(abs(real_I)>=1e-9 && abs(real_I)<1e-6)
            {
                ui->label_11->setText("нА");
                double z=0;
                z=real_I*1e9;
                displaingMes_label_10(z);
            }
            if(abs(real_I)>=1e-6)
            {
                ui->label_11->setText("мкА");
                double z=0;
                z=real_I*1e6;
                displaingMes_label_10(z);
                if (abs(real_I)>=2e-6)
                {
                    connect(this,SIGNAL(changeRangeSIG()),this,SLOT(changeRangeSLOT()));
                    emit changeRangeSIG();
                    ui->label_23->setText("Смените камеру");
                    ui->label_10->setText("ERROR ");
                }
            }

            //********************** Интеграл тока на грубом диапазоне *****************************************
            if(abs(integral_I)<1e-9) // отображение заряда (интеграла тока) с размерностью
            {
                ui->label_8->setText("пКл");
                double z_integral_I = integral_I*1e12; // перевод в 4 цифры с точкой
                displaingMes_label_7(z_integral_I);
            }
            if(abs(integral_I)>=1e-9 && abs(integral_I)<1e-6) // отображение заряда (интеграла тока) с размерностью
            {
                ui->label_8->setText("нКл");
                double z_integral_I = integral_I*1e9; // перевод в 4 цифры с точкой
                displaingMes_label_7(z_integral_I);
            }
            if(abs(integral_I)>=1e-6 && abs(integral_I)<1e-3) // отображение заряда (интеграла тока) с размерностью
            {
                ui->label_8->setText("мкКл");
                double z_integral_I = integral_I*1e6; // перевод в 4 цифры с точкой
                displaingMes_label_7(z_integral_I);
            }
            if(abs(integral_I)>1e-3 && abs(integral_I)<200e-3) // отображение заряда (интеграла тока) с размерностью
            {
                ui->label_8->setText("мКл");
                double z_integral_I = integral_I*1e3; // перевод в 4 цифры с точкой
                displaingMes_label_7(z_integral_I);
            }
            else
                if(abs(integral_I)>=200e-3) ui->label_7->setText("ERROR"); //на грубом диапазоне макс 200 мКл (100000 сек на макс токе 2 мкА)
        }

        //************************* Измерение заряда на среднем и грубом диапазоне ***************************************************

        if(measuring_parameter=="3") //Измерение Заряда
        {
            double x = ADC_V; //значение АЦП в В
            double real_Q1 = x*C2*(1e-12); // реальное значение заряда на конденсаторе в экспоненциальной форме            
            if(abs(real_Q1)<1e-9)
            {
                ui->label_23->setText("Низкая точность - смените диапазон");
            }else{ui->label_23->setText("");}
            if(abs(real_Q1)<1e-9)                                   // до 1000 пКл
            {                
                ui->label_8->setText("пКл");  // отображение
                double z = real_Q1*1e12;
                double q;
                q= real_Q1-Q;
                Q=real_Q1;
                if(time>=2)
                {
                D+=q;  // начинаем считать среднюю производную дозы со второго отсчета (на первом какую то ересь показывает)
                }
                double t=D/(time-1);
                displaingMes_label_7(z);

                if(t<1e-9)
                {
                    ui->label_11->setText("пА");  // отображение
                    double t1=t*1e12;
                    displaingMes_label_10(t1);
                }
                if(t>=1e-9 && t<1e-6)
                {
                    ui->label_11->setText("нА");  // отображение
                    double t1=t*1e9;
                    displaingMes_label_10(t1);
                }
            }
            if(abs(real_Q1)>=1e-9 && abs(real_Q1)<100.2e-9)                                   // до 100 нКл
            {
                ui->label_8->setText("нКл");  // отображение
                double z = real_Q1*1e9;
                double q;
                q= real_Q1-Q;
                Q=real_Q1;
                if(time>=2)
                {
                D+=q;  // начинаем считать среднюю производную дозы со второго отсчета (на первом какую то ересь показывает)
                }
                double t=D/(time-1);
                displaingMes_label_7(z);
                if(t<1e-9)
                {
                    ui->label_11->setText("пА");  // отображение
                    double t1=t*1e12;
                    displaingMes_label_10(t1);
                }
                if(t>=1e-9 && t<1e-6)
                {
                    ui->label_11->setText("нА");  // отображение
                    double t1=t*1e9;
                    displaingMes_label_10(t1);
                }
            }
            if (abs(real_Q1)>=99.9e-9) // если больше 100 нКл - сообщение об ошибке
            {
                connect(this,SIGNAL(changeRangeSIG()),this,SLOT(changeRangeSLOT()));
                emit changeRangeSIG();
                ui->label_23->setText("Смените камеру");
                ui->label_7->setText("ERROR ");
            }
        }

        if(measuring_parameter=="4")     //измерение МАЭД (МПД, МЭД)
        {
            double doseRate = abs(real_I*correctionF*sensibility*760*(273+temperature)/(293*pressure));
            integral_DoseRate+=doseRate;
            if(DOSE_threshold!=0.&&integral_DoseRate>DOSE_threshold) // срабатывания порога по времени
            {
                emit threshold_stop();
                ui->label_4->setText("Достигнут порог по дозе за " + QString::number(time) + " c");
            }
            if(abs(real_I)<2.1e-6)
            {
                if(abs(real_I)<2e-8)
                {
                    ui->label_23->setText("Низкая точность - смените диапазон");
                }else{ui->label_23->setText("");}
                    if(measuring_parameter_dimension == "Гр/с" ||measuring_parameter_dimension == "Зв/с" ||measuring_parameter_dimension == "Р/с")
                    {
                        double z = doseRate*1e12; //  в сек
                        ui->label_25->setText(QString::number(z)); // удалить!!!!!!!
                        if (z<1e3)
                        {
                            ui->label_8->setText("п"+measuring_parameter_dimension);  // отображение
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e3 && z<1e6)
                        {
                            ui->label_8->setText("н"+measuring_parameter_dimension);  // отображение
                            z=z/1e3;
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e6 && z<1e9)
                        {
                            ui->label_8->setText("мк"+measuring_parameter_dimension);  // отображение
                            z=z/1e6;
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e9 && z<1e12)
                        {
                            ui->label_8->setText("м"+measuring_parameter_dimension);  // отображение
                            z=z/1e9;
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e12)
                        {
                            ui->label_8->setText(measuring_parameter_dimension);  // отображение
                            z=z/1e12;
                            displaingMes_label_7(z); // функция отображения
                        }
                    }
                    if(measuring_parameter_dimension == "Гр/мин" ||measuring_parameter_dimension == "Зв/мин" ||measuring_parameter_dimension == "Р/мин")
                    {
                        double z = doseRate*60*1e12; //  в сек
                        ui->label_25->setText(QString::number(z)); // удалить!!!!!!!
                        if (z<1e3)
                        {
                            ui->label_8->setText("п"+measuring_parameter_dimension);  // отображение
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e3 && z<1e6)
                        {
                            ui->label_8->setText("н"+measuring_parameter_dimension);  // отображение
                            z=z/1e3;
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e6 && z<1e9)
                        {
                            ui->label_8->setText("мк"+measuring_parameter_dimension);  // отображение
                            z=z/1e6;
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e9 && z<1e12)
                        {
                            ui->label_8->setText("м"+measuring_parameter_dimension);  // отображение
                            z=z/1e9;
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e12)
                        {
                            ui->label_8->setText(measuring_parameter_dimension);  // отображение
                            z=z/1e12;
                            displaingMes_label_7(z); // функция отображения
                        }
                    }
                    if(measuring_parameter_dimension == "Гр/ч" ||measuring_parameter_dimension == "Зв/ч" ||measuring_parameter_dimension == "Р/ч")
                    {
                        double z = doseRate*3600*1e12; //  в сек
                        ui->label_25->setText(QString::number(z)); // удалить!!!!!!!
                        if (z<1e3)
                        {
                            ui->label_8->setText("п"+measuring_parameter_dimension);  // отображение
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e3 && z<1e6)
                        {
                            ui->label_8->setText("н"+measuring_parameter_dimension);  // отображение
                            z=z/1e3;
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e6 && z<1e9)
                        {
                            ui->label_8->setText("мк"+measuring_parameter_dimension);  // отображение
                            z=z/1e6;
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e9 && z<1e12)
                        {
                            ui->label_8->setText("м"+measuring_parameter_dimension);  // отображение
                            z=z/1e9;
                            displaingMes_label_7(z); // функция отображения
                        }
                        if (z>=1e12)
                        {
                            ui->label_8->setText(measuring_parameter_dimension);  // отображение
                            z=z/1e12;
                            displaingMes_label_7(z); // функция отображения
                        }
                    }
            }
            if (abs(real_I)>=2e-6)
            {
                connect(this,SIGNAL(changeRangeSIG()),this,SLOT(changeRangeSLOT()));
                emit changeRangeSIG();
                ui->label_23->setText("Смените камеру");
                ui->label_7->setText("ERROR ");
            }

            //********************************* интеграл МАЭД *************************************************************************
            //qDebug()<<integral_DoseRate;
                if(integral_DoseRate<1e-9)
                {
                    ui->label_11->setText("п"+dimensionSensibility);
                    double z=integral_DoseRate*1e12;
                    displaingMes_label_10(z);
                }
                if(integral_DoseRate>=1e-9 && integral_DoseRate<1e-6)
                {
                    ui->label_11->setText("н"+dimensionSensibility);
                    double z=integral_DoseRate*1e9;
                    displaingMes_label_10(z);
                }
                if(integral_DoseRate>=1e-6 && integral_DoseRate<1e-3)
                {
                    ui->label_11->setText("мк"+dimensionSensibility);
                    double z=integral_DoseRate*1e6;
                    displaingMes_label_10(z);
                }
                if(integral_DoseRate>=1e-3 && integral_DoseRate<1e-0)
                {
                    ui->label_11->setText("м"+dimensionSensibility);
                    double z=integral_DoseRate*1e3;
                    displaingMes_label_10(z);
                }
                if(integral_DoseRate>=1e0)
                {
                    ui->label_11->setText(dimensionSensibility);
                    double z=integral_DoseRate;
                    displaingMes_label_10(z);
                }
        }

        if(measuring_parameter=="5")     //измерение интеграла МАЭД (МПД, МЭД)
        {
            double doseRate = abs(real_I*correctionF*sensibility*760*(273+temperature)/(293*pressure));
            integral_DoseRate+=doseRate;

            if(DOSE_threshold!=0.&&integral_DoseRate>DOSE_threshold) // срабатывания порога по времени
            {
                emit threshold_stop();
                ui->label_4->setText("Достигнут порог по дозе за " + QString::number(time) + " c");
            }
            if(abs(real_I)<2.1e-6)
            {
                if(abs(real_I)<2e-8)
                {
                    ui->label_23->setText("Низкая точность - смените диапазон");
                }else{ui->label_23->setText("");}
                    if(measuring_parameter_dimension == "Гр/с" ||measuring_parameter_dimension == "Зв/с" ||measuring_parameter_dimension == "Р/с")
                    {
                        double z = doseRate*1e12; //  в сек
                        ui->label_25->setText(QString::number(z)); // удалить!!!!!!!
                        if (z<1e3)
                        {
                            ui->label_11->setText("п"+measuring_parameter_dimension);  // отображение
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e3 && z<1e6)
                        {
                            ui->label_11->setText("н"+measuring_parameter_dimension);  // отображение
                            z=z/1e3;
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e6 && z<1e9)
                        {
                            ui->label_11->setText("мк"+measuring_parameter_dimension);  // отображение
                            z=z/1e6;
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e9 && z<1e12)
                        {
                            ui->label_11->setText("м"+measuring_parameter_dimension);  // отображение
                            z=z/1e9;
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e12)
                        {
                            ui->label_11->setText(measuring_parameter_dimension);  // отображение
                            z=z/1e12;
                            displaingMes_label_10(z); // функция отображения
                        }
                    }
                    if(measuring_parameter_dimension == "Гр/мин" ||measuring_parameter_dimension == "Зв/мин" ||measuring_parameter_dimension == "Р/мин")
                    {
                        double z = doseRate*60*1e12; //  в сек
                        ui->label_25->setText(QString::number(z)); // удалить!!!!!!!
                        if (z<1e3)
                        {
                            ui->label_11->setText("п"+measuring_parameter_dimension);  // отображение
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e3 && z<1e6)
                        {
                            ui->label_11->setText("н"+measuring_parameter_dimension);  // отображение
                            z=z/1e3;
                            displaingMes_label_7(10); // функция отображения
                        }
                        if (z>=1e6 && z<1e9)
                        {
                            ui->label_11->setText("мк"+measuring_parameter_dimension);  // отображение
                            z=z/1e6;
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e9 && z<1e12)
                        {
                            ui->label_11->setText("м"+measuring_parameter_dimension);  // отображение
                            z=z/1e9;
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e12)
                        {
                            ui->label_11->setText(measuring_parameter_dimension);  // отображение
                            z=z/1e12;
                            displaingMes_label_10(z); // функция отображения
                        }
                    }
                    if(measuring_parameter_dimension == "Гр/ч" ||measuring_parameter_dimension == "Зв/ч" ||measuring_parameter_dimension == "Р/ч")
                    {
                        double z = doseRate*3600*1e12; //  в сек
                        ui->label_25->setText(QString::number(z)); // удалить!!!!!!!
                        if (z<1e3)
                        {
                            ui->label_11->setText("п"+measuring_parameter_dimension);  // отображение
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e3 && z<1e6)
                        {
                            ui->label_11->setText("н"+measuring_parameter_dimension);  // отображение
                            z=z/1e3;
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e6 && z<1e9)
                        {
                            ui->label_11->setText("мк"+measuring_parameter_dimension);  // отображение
                            z=z/1e6;
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e9 && z<1e12)
                        {
                            ui->label_11->setText("м"+measuring_parameter_dimension);  // отображение
                            z=z/1e9;
                            displaingMes_label_10(z); // функция отображения
                        }
                        if (z>=1e12)
                        {
                            ui->label_11->setText(measuring_parameter_dimension);  // отображение
                            z=z/1e12;
                            displaingMes_label_10(z); // функция отображения
                        }
                    }
            }
            if (abs(real_I)>=2e-6)
            {
                connect(this,SIGNAL(changeRangeSIG()),this,SLOT(changeRangeSLOT()));
                emit changeRangeSIG();
                ui->label_23->setText("Смените камеру");
                ui->label_7->setText("ERROR ");
            }
            //********************************* интеграл МАЭД *************************************************************************
            //qDebug()<<integral_DoseRate;
                if(integral_DoseRate<1e-9)
                {
                    ui->label_8->setText("п"+dimensionSensibility);
                    double z=integral_DoseRate*1e12;
                    displaingMes_label_7(z);
                }
                if(integral_DoseRate>=1e-9 && integral_DoseRate<1e-6)
                {
                    ui->label_8->setText("н"+dimensionSensibility);
                    double z=integral_DoseRate*1e9;
                    displaingMes_label_7(z);
                }
                if(integral_DoseRate>=1e-6 && integral_DoseRate<1e-3)
                {
                    ui->label_8->setText("мк"+dimensionSensibility);
                    double z=integral_DoseRate*1e6;
                    displaingMes_label_7(z);
                }
                if(integral_DoseRate>=1e-3 && integral_DoseRate<1e-0)
                {
                    ui->label_8->setText("м"+dimensionSensibility);
                    double z=integral_DoseRate*1e3;
                    displaingMes_label_7(z);
                }
                if(integral_DoseRate>=1e0)
                {
                    ui->label_8->setText(dimensionSensibility);
                    double z=integral_DoseRate;
                    displaingMes_label_7(z);
                }
        }

        if(measuring_parameter=="6")     //измерение дозы на конденсаторе на среднем и грубом диапазоне
        {
            double x = ADC_V; //значение АЦП в В
            double real_Q1 = x*C2*(1e-12); // реальное значение заряда на конденсаторе в экспоненциальной форме
            double realDose = abs(real_Q1*correctionF*sensibility*760*(273+temperature)/(293*pressure));

            if(DOSE_threshold!=0.&&realDose>DOSE_threshold) // срабатывания порога по времени
            {
                emit threshold_stop();
                ui->label_4->setText("Достигнут порог по дозе за " + QString::number(time) + " c");
            }
            if(abs(real_Q1)<1e-9)
            {
                ui->label_23->setText("Низкая точность - смените диапазон");
            }else{ui->label_23->setText("");}
            if(abs(real_Q1)<100.2e-9)                                   // до 100 нКл
            {
                if(realDose<1e-6)
                {
                    ui->label_8->setText("н"+dimensionSensibility);  // отображение
                    double z = realDose*1e9;
                    displaingMes_label_7(z);
                }
                if(realDose>=1e-6 && realDose<1e-3)
                {
                    ui->label_8->setText("мк"+dimensionSensibility);  // отображение
                    double z = realDose*1e6;
                    displaingMes_label_7(z);
                }
                if(realDose>=1e-3 && realDose<1e0)
                {
                    ui->label_8->setText("м"+dimensionSensibility);  // отображение
                    double z = realDose*1e3;
                    displaingMes_label_7(z);
                }
                if(realDose>=1e0 && realDose<1e3)
                {
                    ui->label_8->setText(""+dimensionSensibility);  // отображение
                    double z = realDose*1e0;
                    displaingMes_label_7(z);
                }
                double DOSE = realDose-D; //
                D=realDose;
                if(time>=2)
                {
                Q+=DOSE;  // начинаем считать среднюю производную дозы со второго отсчета (на первом какую то ересь показывает)
                }
                if(measuring_parameter_dimension == "Гр/с" ||measuring_parameter_dimension == "Зв/с" ||measuring_parameter_dimension == "Р/с")
                {
                    double DOSE1=abs(Q/(time-1));
                    if(DOSE1<1e-9)
                    {
                        ui->label_11->setText("п"+dimensionSensibility+"/c");  // отображение
                        double z = DOSE1*1e12;
                        displaingMes_label_10(z);
                    }
                    if(DOSE1>=1e-9 && DOSE1<1e-6)
                    {
                        ui->label_11->setText("н"+dimensionSensibility+"/c");  // отображение
                        double z = DOSE1*1e9;
                        displaingMes_label_10(z);
                    }
                    if(DOSE1>=1e-6 && DOSE1<1e-3)
                    {
                        ui->label_11->setText("мк"+dimensionSensibility+"/c");  // отображение
                        double z = DOSE1*1e6;
                        displaingMes_label_10(z);
                    }
                    if(DOSE1>=1e-3 && DOSE1<1e0)
                    {
                        ui->label_11->setText("м"+dimensionSensibility+"/c");  // отображение
                        double z = DOSE1*1e3;
                       displaingMes_label_10(z);
                    }
                }
                if(measuring_parameter_dimension == "Гр/мин" ||measuring_parameter_dimension == "Зв/мин" ||measuring_parameter_dimension == "Р/мин")
                {
                    double DOSE1=abs(Q*60/(time-1));
                    if(DOSE1<1e-9)
                    {
                        ui->label_11->setText("п"+dimensionSensibility+"/мин");  // отображение
                        double z = DOSE1*1e12;
                        displaingMes_label_10(z);
                    }
                    if(DOSE1>=1e-9 && DOSE1<1e-6)
                    {
                        ui->label_11->setText("н"+dimensionSensibility+"/мин");  // отображение
                        double z = DOSE1*1e9;
                        displaingMes_label_10(z);
                    }
                    if(DOSE1>=1e-6 && DOSE1<1e-3)
                    {
                        ui->label_11->setText("мк"+dimensionSensibility+"/мин");  // отображение
                        double z = DOSE1*1e6;
                        displaingMes_label_10(z);
                    }
                    if(DOSE1>=1e-3 && DOSE1<1e0)
                    {
                        ui->label_11->setText("м"+dimensionSensibility+"/мин");  // отображение
                        double z = DOSE1*1e3;
                       displaingMes_label_10(z);
                    }
                    if(DOSE1>1e0)
                    {
                        ui->label_11->setText(""+dimensionSensibility+"/мин");  // отображение
                        double z = DOSE1;
                       displaingMes_label_10(z);
                    }
                }
                if(measuring_parameter_dimension == "Гр/ч" ||measuring_parameter_dimension == "Зв/ч" ||measuring_parameter_dimension == "Р/ч")
                {
                    double DOSE1=abs(Q*3600/(time-1));
                    if(DOSE1<1e-9)
                    {
                        ui->label_11->setText("п"+dimensionSensibility+"/ч");  // отображение
                        double z = DOSE1*1e12;
                        displaingMes_label_10(z);
                    }
                    if(DOSE1>=1e-9 && DOSE1<1e-6)
                    {
                        ui->label_11->setText("н"+dimensionSensibility+"/ч");  // отображение
                        double z = DOSE1*1e9;
                        displaingMes_label_10(z);
                    }
                    if(DOSE1>=1e-6 && DOSE1<1e-3)
                    {
                        ui->label_11->setText("мк"+dimensionSensibility+"/ч");  // отображение
                        double z = DOSE1*1e6;
                        displaingMes_label_10(z);
                    }
                    if(DOSE1>=1e-3 && DOSE1<1e0)
                    {
                        ui->label_11->setText("м"+dimensionSensibility+"/ч");  // отображение
                        double z = DOSE1*1e3;
                       displaingMes_label_10(z);
                    }
                    if(DOSE1>1e0)
                    {
                        ui->label_11->setText(""+dimensionSensibility+"/ч");  // отображение
                        double z = DOSE1;
                       displaingMes_label_10(z);
                    }
                }
            }
            if (abs(real_Q1)>=99.99e-9)  // если больше 100 нКл - сообщение об ошибке
            {
                connect(this,SIGNAL(changeRangeSIG()),this,SLOT(changeRangeSLOT()));
                emit changeRangeSIG();
                ui->label_23->setText("Смените диапазон");
                ui->label_7->setText("ERROR ");
            }
        }
    }

    //****************************************************************************************************
    //************************** Отображение СКО и среднего **********************************************
    //****************************************************************************************************
    //*************************************** СКО  *************************
    if(standart_deviation_show=="1" && (measuring_parameter=="1")) // СКО ТОКа
    {
        ui->label_13->setText("СКО= ");
        double x = ADC_V;
        double real_I=0;

        if (measurement_range =="1")  //Чувствительный
        {
            x = ADC_V;
            real_I = x/(R3*1000000000);
        }
        if (measurement_range =="2")  //Средний
        {
            x = ADC_V;
            real_I = x/(R2*1000000);
        }
        if (measurement_range =="3")  //Грубый
        {
            x = ADC_V;
            real_I = x/(R1*1000000);
        }
        av+=real_I;
        AverageMes=av/(time);
        otkl+=qPow((real_I-AverageMes),2);
        if(time>1)
        {
            double z=qPow(otkl/(time-1),0.5);
            StandartDeviation=abs(z/AverageMes);
            ui->label_14->setText(QString::number(StandartDeviation*100,'f',4)+"%");
            //ui->label_14->setText(QString::number(StandartDeviation,'f'));
        }
        if (abs(AverageMes)<1e-9)
        {
            ui->label_27->setText("пА");
            double z=AverageMes*1e12;
            displaingMes_label_12(z);
        }
        if (abs(AverageMes)>=1e-9 && abs(AverageMes)<1e-6)
        {
            ui->label_27->setText("нА");
            double z=AverageMes*1e9;
            displaingMes_label_12(z);
        }
        if (abs(AverageMes)>=1e-6 && abs(AverageMes)<1e-3)
        {
            ui->label_27->setText("мкА");
            double z=AverageMes*1e6;
            displaingMes_label_12(z);
        }
        if (abs(AverageMes)>=1e-3 && abs(AverageMes)<1e0)
        {
            ui->label_27->setText("мА");
            double z=AverageMes*1e3;
            displaingMes_label_12(z);
        }
        if (abs(AverageMes)>=1e0)
        {
            ui->label_27->setText("А");
            double z=AverageMes;
            displaingMes_label_12(z);
        }        
    }
//***********************************************************************************************************
    if(standart_deviation_show=="1" && (measuring_parameter=="4" )) // СКО МПД
    {

        ui->label_13->setText("СКО= ");
        double x = ADC_V;
        double real_I=0;
        if (measurement_range =="1")  //Чувствительный
        {
            x = ADC_V;
            real_I = x/(R3*1000000000);
        }
        if (measurement_range =="2")  //Средний
        {
            x = ADC_V;
            real_I = x/(R2*1000000);
        }
        if (measurement_range =="3")  //Грубый
        {
            x = ADC_V;
            real_I = x/(R1*1000000);
        }
        double doseRate = abs(real_I*correctionF*sensibility*760*(273+temperature)/(293*pressure));
        av+=doseRate;
        AverageMes=av/(time);
        otkl+=qPow((doseRate-AverageMes),2);
        if(time>1)
        {
            double z=qPow(otkl/(time-1),0.5);
            StandartDeviation=abs(z/AverageMes);
            ui->label_14->setText(QString::number(StandartDeviation*100,'f',4)+"%");
        }


        if(measuring_parameter_dimension == "Гр/с" ||measuring_parameter_dimension == "Зв/с" ||measuring_parameter_dimension == "Р/с")
        {
            if (AverageMes<1e-9)
            {
                ui->label_27->setText("п"+measuring_parameter_dimension);
                double z=AverageMes*1e12;
                displaingMes_label_12(z);
            }
            if (AverageMes>=1e-9 && AverageMes<1e-6)
            {
                ui->label_27->setText("н"+ measuring_parameter_dimension);
                double z=AverageMes*1e9;
                displaingMes_label_12(z);
            }
            if (AverageMes>=1e-6 && AverageMes<1e-3)
            {
                ui->label_27->setText("мк"+measuring_parameter_dimension);
                double z=AverageMes*1e6;
                displaingMes_label_12(z);
            }
            if (AverageMes>=1e-3 && AverageMes<1e0)
            {
                ui->label_27->setText("м"+measuring_parameter_dimension);
                double z=AverageMes*1e3;
                displaingMes_label_12(z);
            }
            if (AverageMes>=1e0)
            {
                ui->label_27->setText(""+measuring_parameter_dimension);
                double z=AverageMes;
                displaingMes_label_12(z);
            }
        }
        if(measuring_parameter_dimension == "Гр/мин" ||measuring_parameter_dimension == "Зв/мин" ||measuring_parameter_dimension == "Р/мин")
        {
            double y=AverageMes*60;            
            if (y<1e-9)
            {
                ui->label_27->setText("п"+measuring_parameter_dimension);
                double z=y*1e12;
                displaingMes_label_12(z);
            }
            if (y>=1e-9 && y<1e-6)
            {
                ui->label_27->setText("н"+ measuring_parameter_dimension);
                double z=y*1e9;
                displaingMes_label_12(z);
            }
            if (y>=1e-6 && y<1e-3)
            {
                ui->label_27->setText("мк"+measuring_parameter_dimension);
                double z=y*1e6;
                displaingMes_label_12(z);
            }
            if (y>=1e-3 && y<1e0)
            {
                ui->label_27->setText("м"+measuring_parameter_dimension);
                double z=y*1e3;
                displaingMes_label_12(z);
            }
            if (y>=1e0)
            {
                ui->label_27->setText(""+measuring_parameter_dimension);
                double z=y;
                displaingMes_label_12(z);
            }
        }
        if(measuring_parameter_dimension == "Гр/ч" ||measuring_parameter_dimension == "Зв/ч" ||measuring_parameter_dimension == "Р/ч")
        {
            double y=AverageMes*3600;
            if (y<1e-9)
            {
                ui->label_27->setText("п"+measuring_parameter_dimension);
                double z=y*1e12;
                displaingMes_label_12(z);
            }
            if (y>=1e-9 && y<1e-6)
            {
                ui->label_27->setText("н"+ measuring_parameter_dimension);
                double z=y*1e9;
                displaingMes_label_12(z);
            }
            if (y>=1e-6 && y<1e-3)
            {
                ui->label_27->setText("мк"+measuring_parameter_dimension);
                double z=y*1e6;
                displaingMes_label_12(z);
            }
            if (y>=1e-3 && y<1e0)
            {
                ui->label_27->setText("м"+measuring_parameter_dimension);
                double z=y*1e3;
                displaingMes_label_12(z);
            }
            if (y>=1e0)
            {
                ui->label_27->setText(""+measuring_parameter_dimension);
                double z=y;
                displaingMes_label_12(z);
            }
        }
    }
//********************************************--- Режим КИ ---***********************************
    if (ui->label_31->text()=="DELTA=")
    {
        double x = ADC_V;
        double real_I=0;
        if (measurement_range =="1")  //Чувствительный
        {
            x = ADC_V;
            real_I = x/(R3*1000000000);
        }
        if (measurement_range =="2")  //Средний
        {
            x = ADC_V;
            real_I = x/(R2*1000000);
        }
        if (measurement_range =="3")  //Грубый
        {
            x = ADC_V;
            real_I = x/(R1*1000000);
        }
        double doseRate = abs(real_I*correctionF*sensibility*760*(273+temperature)/(293*pressure));
        av+=doseRate;
        AverageMes=av/(time);
        double delta=abs(100*(doseRateFull-AverageMes)/doseRateFull);
        //qDebug()<<doseRateFull<<AverageMes;
        ui->label_32->setText(QString::number(delta,'f',3)+"%");
    }

}

void Measuring::on_pushButton_8_clicked() //кнопка стоп
{
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_4->setEnabled(true);
    if(CS_flag==1)
    {
        ui->pushButton_4->setStyleSheet("color: white; background-color: qradialgradient(cx: 0.5, cy: 0.5, radius: 2, fx: 0.5, fy: 1, stop: 0 rgba(255,30,30,255), stop: 0.2 rgba(255,30,30,144), stop: 0.4 rgba(255,30,30,32));border:1px solid black;border-radius:5px;");
    }else {
        ui->pushButton_4->setStyleSheet("color: black;");
    }
    ui->pushButton_5->setEnabled(true);
    ui->pushButton_7->setEnabled(true);

    //time=0;
    timer->stop();
    av = 0;
    otkl=0;
    integral_I=0;
    Time_threshold=0;
    DOSE_threshold=0;
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("/home/pi/chambers.db");
        QSqlQuery query(db);
        if(!query.exec("SELECT *FROM config"))
        {
            qDebug()<<"Unable execute query charge";
        }
        if (query.first())
        {
            if((query.value("measuring_parameter").toString()=="3"||query.value("measuring_parameter").toString()=="6")&&query.value("measurement_range").toString()=="1") //заряд чувствительный
            {
                QByteArray hA0;
                hA0[0]=0x2a;//28
                hA0[1]=0x30;//30 код реле - К7+К8 - заряд (по кнопке старт) в режиме измерения заряда на чувствительном диапазоне
                emit sendToPortMainW(hA0);
                emit sendToPortMainW(hA0);
            }
            if((query.value("measuring_parameter").toString()=="3"||query.value("measuring_parameter").toString()=="6")&&query.value("measurement_range").toString()=="2") //заряд средний
            {
                QByteArray hE0;
                hE0[0]=0x2e;//2c
                hE0[1]=0x30;//30 код реле - К6+К8 - заряд (по кнопке старт) в режиме измерения заряда на среднем и грубом диапазоне
                emit sendToPortMainW(hE0);
                emit sendToPortMainW(hE0);
            }
            if((query.value("measuring_parameter").toString()=="3"||query.value("measuring_parameter").toString()=="6")&&query.value("measurement_range").toString()=="3") //заряд грубый
            {
                QByteArray hE0;
                hE0[0]=0x2e;//2c
                hE0[1]=0x30;//30 код реле - К6+К8 - заряд (по кнопке старт) в режиме измерения заряда на среднем и грубом диапазоне
                emit sendToPortMainW(hE0);
                emit sendToPortMainW(hE0);
            }
        }
    }
}

void Measuring::changeRangeSLOT() // функция по сигналу от измерения по достижению макс значений тока.
{
    time=0;
    timer->stop();
    ui->label_4->setText(QString::number(time)); // отображение времени
    av = 0;
    otkl=0;
}


void Measuring::on_pushButton_4_clicked() // Кнопка КИ
{
   CSbut=!CSbut; // булевое условие не нажатия на кнопку
   if (CSbut==1)
   {
       CS_flag=1;
       ui->pushButton_4->setStyleSheet("color: white;");
       /*
       QPalette pal =palette();
       pal.setColor(QPalette::Background,Qt::black);
       ui->pushButton_4->setAutoFillBackground(true);
       ui->pushButton_4->setPalette(pal);
       ui->pushButton_4->update();
        */

       ui->pushButton_4->setStyleSheet("color: white; background-color: qradialgradient(cx: 0.5, cy: 0.5, radius: 2, fx: 0.5, fy: 1, stop: 0 rgba(255,30,30,255), stop: 0.2 rgba(255,30,30,144), stop: 0.4 rgba(255,30,30,32));border:1px solid black;border-radius:5px;");
       Sources *sour = new Sources(this);
       sour->setModal(true);
       sour->init();
       sour->showFullScreen();
       connect(sour,SIGNAL(setCS(QString)),this,SLOT(setCS(QString)));

   }
   else
   {
       ui->pushButton_4->setStyleSheet("color:black");
       CS_flag=0;
       ui->label_31->setText("");
        ui->label_32->setText("");
   }
}

void Measuring::setCS(QString number) //функция приема сигнала о включении режима контрольного источника
{
    if (QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("/home/pi/chambers.db");
        if(!db.open())
        {
            qDebug()<<"DB was not opened";
        }
        QSqlQuery query(db);
        if(!query.exec("SELECT * FROM sources"))
        {
            qDebug()<<"unable execute query";
        }
        while(query.next())
        {
            if(query.value("number").toString()==number)
            {
                //dateVerificationCS
                QDate dateVerificationCSd=QDate::fromString(query.value("sourceDateVerification").toString(),"dd.MM.yyyy");
                qDebug()<<dateVerificationCSd;
                half_lifeCS=query.value("half_life").toDouble();
                doseRateVerificationCS=query.value("doseRateVerification").toDouble();
                doseRateCurrentCS=doseRateVerificationCS*pow(2,-1*(QDate::currentDate().toJulianDay()-dateVerificationCSd.toJulianDay())/(365*half_lifeCS));
                doseRateDimension = query.value("doseRateDimension").toString();
                QString strDimension = doseRateDimension.remove(2,doseRateDimension.size());
                qDebug()<<strDimension;
                if (strDimension=="мк")
                {
                    doseRateFull = doseRateCurrentCS*1E-06;  // Доза в Гр/с (Зв/с) от источника на данный момент (расчетная-эталонная)
                }
                else
                {
                    doseRateFull = doseRateCurrentCS*1E-03; // Доза в Гр/с (Зв/с) от источника на данный момент (расчетная-эталонная)
                }
            }
        }
    }

    Measuring_parameters *mesPar = new Measuring_parameters(this);
    mesPar->init_CS();
    connect(this,SIGNAL(setCStoMesPar()),mesPar,SLOT(setCStoMesPar()));
    connect(mesPar,SIGNAL(updateMesPar()),this,SLOT(updateMesPar()));
    emit setCStoMesPar();
    ui->label_31->setText("DELTA=");
    //double delta=abs(100*(doseRateFull-AverageMes)/doseRateFull);
    //ui->label_32->setText(QString::number(delta,'f',3));
}




