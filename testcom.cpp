#include "testcom.h"
#include "ui_testcom.h"

TestCom::TestCom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestCom)
{
    ui->setupUi(this);

    QTimer *time = new QTimer(this); // таймер реально прошедшего времени
    connect(time,SIGNAL(timeout()),this,SLOT(timeout_time()));
    time->start(1000);

    QTimer *timerADC = new QTimer(this);
    connect(timerADC,SIGNAL(timeout()),this,SLOT(timeout_timerADC()));
    timerADC->start(99);

    T=0; ADC_V=0; ADC_offset=0; T_ADC=0; averADC_V=0; l1=0; l2=0; averCounts=5;
    C2=0; C2_1=0; C2_2=0; C2_3=0; C2_4=0;
    x=0; // переменная для рассчета среднего значения от ADC
    y=0;  // переменная для рассчета среднего среднего значения от ADC
    ui->label_4->setText(QString::number(T));
    l0 = 8388608; // - константа соответствующая разряду АЦП при нулевом напряжении;

    QFile conf("/home/pi/advconfig.txt");
    QStringList strList1;
    if(conf.exists()&&conf.open(QIODevice::ReadOnly))
    {
        while(!conf.atEnd())
        {
            strList1.append(conf.readLine());
        }
        conf.close();
    }
    warmingTime=strList1[0].toInt(); // - время прогрева после включения аппаратуры
    timeoutC2=strList1[1].toInt();   // - таймаут выхода на режим после изменения параметров
    timeoutTest=strList1[3].toInt(); // - таймаут теста (между измерениями или после включения реле???)
    timeoutI=strList1[4].toInt();    // - таймаут тока
    Kintegral=strList1[6].toInt();   // - К - коэффициент интеграции отсчетов (Т=К*период опроса порта)

    QFile test("/home/pi/test.txt");
    QStringList strList2;
    if (test.exists()&&test.open(QIODevice::ReadOnly))
    {
        while(!test.atEnd())
        {
            strList2.append(test.readLine());
        }
        test.close();
    }
    averNum=strList2[12].toInt(); // - задание количества отсчетов при усреднении

    QFile config("/home/pi/config.txt");
    QStringList strList3;
    if(config.exists()&&config.open(QIODevice::ReadOnly))
    {
        while(!config.atEnd())
        {
            strList3.append(config.readLine());
        }
        config.close();
    }
    R1=strList3[1].toDouble();
    warmingTime=warmingTime+6;
    ui->progressBar->setValue(0);

    h00[0]=0x20;            //            *********** static_cast<char>(0x89); ********************** //20
    h00[1]=0x30; //30 код реле - выкл все
    h21[0]=34; //22
    h21[1]=49; //31 код реле - К5+К9 - замыкается 1 МОм и охранный с измерительным электроды
    h29[0]=34;//22
    h29[1]=57;//39 код реле - К4+К5+К9 - замыкается 1 МОм + охранный с измерительным электроды + ИОН на 100 МОм(для калибровки 100 МОм)
    h15[0]=33;//21
    h15[1]=53;//35 код реле - К2+К3+К9 - замыкается 100 МОм + охранный с измерительным электроды + ИОН на 10 ГОм(для калибровки 10 ГОм)
    h61[0]=38;//26
    h61[1]=49;//31 код реле - К5+К6+К7+К9 - сбрасывается заряд на С1 и С2 + охранный с измерительным электроды
    h45[0]=36;//24
    h45[1]=53;//35 код реле - К2+К6+К9 - заряжается конденсатор С2(К6) (47нФ) током от ИОН через 10 ГОм(К2) (250пА)+ охранный с измерительным электроды(К9)
    h03[0]=32;//20
    h03[1]=51;//33 код реле - К1+К9 - замыкается 10 ГОм + охранный с измерительным электроды - измерение входного тока
    hC2[0]=44;//2c
    hC2[1]=50;//32 код реле - К1+К7+К8 - Чувствительный диапазон + фильтр шумов (С1 - 560пФ)
    h82[0]=40;//28
    h82[1]=50;//32 код реле - К1+К8 - Чувствительный диапазон
    h90[0]=41;//29
    h90[1]=48;//30 код реле - К3+К8 - Средний диапазон (на среднем диапазоне фильтр шумов не подключается)
    hA0[0]=42;//2a
    hA0[1]=48;//30 код реле - К5+К7+К8 - Грубый диапазон + фильтр шумов (С1 - 560пФ) (на грубом диапазоне фильтр шумов не отключается)
                        // + сброс заряда (по кнопке стоп) в режиме измерения заряда на чувствительном диапазоне
    h80[0]=40;//28
    h80[1]=48;//30 код реле - К7+К8 - заряд (по кнопке старт) в режиме измерения заряда на чувствительном диапазоне
    hE0[0]=46;//2e
    hE0[1]=48;//30 код реле - К5+К6+К7+К8 - сброс заряда (по кнопке стоп) в режиме измерения заряда на чувствительном диапазоне
    hC0[0]=44;//2c
    hC0[1]=48;//30 код реле - К6+К8 - заряд (по кнопке старт) в режиме измерения заряда на среднем и грубом диапазоне
}

TestCom::~TestCom()
{
    delete ui;
    qDebug()<<"destruptorTest";    
}

void TestCom::delay(int i)
{
    while(i>0) i--;
}

void TestCom::timeout_time() // функция таймаута таймера реального времени
{
    ui->label_4->setText(QString::number(T));
    T+=1;
//********************************************************** Основная функция **********************************

//*************************************** Выставление кода ЦАП на усилителе, для нулевого входного смещения усилителя***************
    if(T<warmingTime)
    {
        ui->label_6->setText("Прогрев");
    }
    if (T==warmingTime)
    {
        emit sendToCom(h21);
        delay(10000);
        emit sendToCom(h21);
    }
    if (T==warmingTime+timeoutTest)
    {
        emit sendToCom(h00);
        delay(10000);
        emit sendToCom(h00);
        ui->label_6->setText("Самотестирование");
        ui->progressBar->setValue(0);
    }
    if (T==warmingTime+2*timeoutTest)
    {
        ui->label_6->setText("Определение напряжения смещения");
        emit sendToCom(h21);
        delay(10000);
        emit sendToCom(h21);
        ui->progressBar->setValue(0);
    }
    if (T==warmingTime+3*timeoutTest)
    {
        bytesDAC[0]=0x40;
        bytesDAC[1]=0x50;
        bytesDAC[2]=0x60;
        emit sendToCom(bytesDAC);
        delay(10000);
        emit sendToCom(bytesDAC);
        ui->progressBar->setValue(10);
    }
    if (T>=warmingTime+4*timeoutTest && T<=warmingTime+4*timeoutTest+averCounts)
    {
        ui->progressBar->setValue(20+5*(T-(warmingTime+4*timeoutTest)));
        y+=averADC_V;
        //qDebug()<<y;
        if(T==warmingTime+4*timeoutTest+averCounts)
        {
            l1=y/(averCounts+1); // зреднее значение АЦП в разрядах
            qDebug()<<"l1"<<l1;
            y=0;
        }
       // l1=QString::number(averADC_V).toLong();
    }
    if (T==warmingTime+5*timeoutTest+averCounts)
    {
        ui->progressBar->setValue(50);
        bytesDAC[0]=0x4f;
        bytesDAC[1]=0x5f;
        bytesDAC[2]=0x6f;
        emit sendToCom(bytesDAC);
        delay(10000);
        emit sendToCom(bytesDAC);
    }
    if (T>=warmingTime+6*timeoutTest+averCounts && T<=warmingTime+6*timeoutTest+2*averCounts)
    {
        ui->progressBar->setValue(50+5*(T-(warmingTime+6*timeoutTest+averCounts)));
        y+=averADC_V;
        if(T==warmingTime+6*timeoutTest+2*averCounts)
        {
            l2=y/(averCounts+1); // зреднее значение АЦП в разрядах
            qDebug()<<"l2"<<l2;
            y=0;
        }

        //l2=QString::number(averADC_V).toLong();
    }
    if (T==warmingTime+6*timeoutTest+2*averCounts)
    {
        ui->progressBar->setValue(80);
        qDebug()<<l0<<l1<<l2;
        d=abs((static_cast<double>(l0)-static_cast<double>(l1))/(static_cast<double>(l2)-static_cast<double>(l1)));
        qDebug()<<d;
        DAC=0xfff*d;
        qDebug()<<DAC;
        long DAC1=static_cast<long>(DAC);
        strDAC=QString::number(DAC1,16);
        QString DAC4="",DAC5="",DAC6="";
        DAC4="4"+strDAC[0];
        DAC5="5"+strDAC[1];
        DAC6="6"+strDAC[2];
        bytesDAC[0]=static_cast<char>(DAC4.toUInt(nullptr,16));
        bytesDAC[1]=static_cast<char>(DAC5.toUInt(nullptr,16));
        bytesDAC[2]=static_cast<char>(DAC6.toUInt(nullptr,16));
        QFile cal("/home/pi/calibration.txt");
        QStringList strList;

        if(cal.exists()&&cal.open(QIODevice::ReadOnly))
        {
            while(!cal.atEnd())
            {
                strList.append(cal.readLine());
            }
            cal.close();
        }
        strList[5]=QString::number(DAC1)+"\n";

        if(cal.exists()&&cal.open(QIODevice::WriteOnly))
        {
            QTextStream stream(&cal);
            foreach(QString str,strList)
            {
                stream<<str;
            }
            cal.close();
        }

    }
    if (T==warmingTime+6*timeoutTest+2*averCounts)
    {
        emit sendToCom(bytesDAC);
        delay(10000);
        emit sendToCom(bytesDAC);
    }
    if (T>=warmingTime+7*timeoutTest+2*averCounts && T<=warmingTime+7*timeoutTest+3*averCounts)
    {
        y+=averADC_V;
        if(T==warmingTime+7*timeoutTest+3*averCounts)
        {
            ADC_offset=y/(averCounts+1); // зреднее значение АЦП в разрядах
            y=0;
        }
    }

    if (T==warmingTime+7*timeoutTest+3*averCounts)
    {
        qDebug()<<QString::number((ADC_offset-l0)*0.0000003,'f',7);
        ui->progressBar->setValue(100);
        QFile cal("/home/pi/calibration.txt");
        QStringList strList;
        if(cal.exists()&&cal.open(QIODevice::ReadOnly))
        {
            while(!cal.atEnd())
            {
                strList.append(cal.readLine());
            }
            cal.close();
        }
        strList[0]=QString::number((ADC_offset-l0)*0.0000003,'d',7)+"\n";

        if(cal.exists()&&cal.open(QIODevice::WriteOnly))
        {
            QTextStream stream(&cal);
            foreach(QString str,strList)
            {
                stream<<str;
            }
            cal.close();
        }
    }
//***************** Калибровка R2 **********************************************************************************
    if (T==warmingTime+7*timeoutTest+3*averCounts)
    {
        ui->progressBar->setValue(0);
        ui->label_6->setText("Калибровка R2");
        emit sendToCom(h29);
        delay(10000);
        emit sendToCom(h29);
    }
    if (T==warmingTime+8*timeoutTest+3*averCounts)
    {
        ui->progressBar->setValue(20);
    }
    if (T>=warmingTime+8*timeoutTest+3*averCounts && T<=warmingTime+8*timeoutTest+4*averCounts)
    {
        ui->progressBar->setValue(20+10*(T-(warmingTime+8*timeoutTest+3*averCounts)));
        y+=averADC_V;
        if(T==warmingTime+8*timeoutTest+4*averCounts)
        {
            l1=y/(averCounts+1); // зреднее значение АЦП в разрядах
            R2=((l0*R1*1000000)/(l1-l0) - 100000)/1000000;   // - калибровка R2, в МОм
            qDebug()<<"R2"<<R2;
            y=0;

            QFile cal("/home/pi/calibration.txt");
            QStringList strList;
            if(cal.exists()&&cal.open(QIODevice::ReadOnly))
            {
                while(!cal.atEnd())
                {
                    strList.append(cal.readLine());
                }
                cal.close();
            }
            strList[1]=QString::number(R2,'d',3)+"\n";

            if(cal.exists()&&cal.open(QIODevice::WriteOnly))
            {
                QTextStream stream(&cal);
                foreach(QString str,strList)
                {
                    stream<<str;
                }
                cal.close();
                ui->progressBar->setValue(100);
            }
        }
    }
//****************************** Калибровка R3 ***************************************************************
    if (T==warmingTime+8*timeoutTest+4*averCounts)
    {
        ui->progressBar->setValue(0);
        ui->label_6->setText("Калибровка R3");
        emit sendToCom(h15);
        delay(10000);
        emit sendToCom(h15);
    }
    if (T==warmingTime+9*timeoutTest+4*averCounts)
    {
        ui->progressBar->setValue(20);
    }
    if (T>=warmingTime+9*timeoutTest+4*averCounts && T<=warmingTime+9*timeoutTest+5*averCounts)
    {
        ui->progressBar->setValue(20+10*(T-(warmingTime+9*timeoutTest+4*averCounts)));
        y+=averADC_V;
        if(T==warmingTime+9*timeoutTest+5*averCounts)
        {
            l1=y/(averCounts+1); // зреднее значение АЦП в разрядах
            R3=((l0*R2*1000000)/(l1-l0) - 100000)/1000000000; // - калибровка R3, в ГОм
            qDebug()<<"R3"<<R3;
            y=0;

            QFile cal("/home/pi/calibration.txt");
            QStringList strList;
            if(cal.exists()&&cal.open(QIODevice::ReadOnly))
            {
                while(!cal.atEnd())
                {
                    strList.append(cal.readLine());
                }
                cal.close();
            }
            strList[2]=QString::number(R3,'d',3)+"\n";

            if(cal.exists()&&cal.open(QIODevice::WriteOnly))
            {
                QTextStream stream(&cal);
                foreach(QString str,strList)
                {
                    stream<<str;
                }
                cal.close();
                ui->progressBar->setValue(100);
            }
        }
    }
//********************************** Калибровка С2 *********************************************

    //************************************************* Первая иттерация *****************
    if (T==warmingTime+9*timeoutTest+5*averCounts)
    {
        ui->progressBar->setValue(0);
        ui->label_6->setText("Калибровка С2");
        emit sendToCom(h61);
        delay(10000);
        emit sendToCom(h61);// сброс заряда
    }
    if (T==warmingTime+10*timeoutTest+5*averCounts)
    {
        ui->progressBar->setValue(20);
    }
    if (T==warmingTime+10*timeoutTest+5*averCounts)
    {
        emit sendToCom(h45);
        delay(10000);
        emit sendToCom(h45);   // зарядка конденсатора
    }
    if (T==warmingTime+11*timeoutTest+5*averCounts)
    {
        l1=static_cast<long>(averADC_V);
        qDebug()<<"l1"<<l1;

    }
    if (T==warmingTime+17*timeoutTest+5*averCounts)
    {
        l2=static_cast<long>(ADC_V);
        qDebug()<<"l2"<<l2;
        C2_1=(l0*(17*timeoutTest-11*timeoutTest)/((l2-l1)*R3));
        qDebug()<<"C2_1"<<C2_1;
    }

    //************************************************* Вторая иттерация *****************
    if (T==warmingTime+18*timeoutTest+5*averCounts)
    {
        emit sendToCom(h61);
        emit sendToCom(h61);// сброс заряда
    }
    if (T==warmingTime+19*timeoutTest+5*averCounts)
    {
        ui->progressBar->setValue(40);
    }
    if (T==warmingTime+19*timeoutTest+5*averCounts)
    {
        emit sendToCom(h45);
        delay(10000);
        emit sendToCom(h45);// зарядка конденсатора
    }
    if (T==warmingTime+20*timeoutTest+5*averCounts)
    {
        l1=static_cast<long>(averADC_V);
        qDebug()<<"l1"<<l1;

    }
    if (T==warmingTime+26*timeoutTest+5*averCounts)
    {
        l2=static_cast<long>(ADC_V);
        qDebug()<<"l2"<<l2;
        C2_2=(l0*(26*timeoutTest-20*timeoutTest)/((l2-l1)*R3));
        qDebug()<<"C2_2"<<C2_2;
    }
    //************************************************* Третья иттерация *****************
    if (T==warmingTime+27*timeoutTest+5*averCounts)
    {
        emit sendToCom(h61);
        delay(10000);
        emit sendToCom(h61);// сброс заряда
    }
    if (T==warmingTime+28*timeoutTest+5*averCounts)
    {
        ui->progressBar->setValue(60);
    }
    if (T==warmingTime+28*timeoutTest+5*averCounts)
    {
        emit sendToCom(h45);
        delay(10000);
        emit sendToCom(h45);// зарядка конденсатора
    }
    if (T==warmingTime+29*timeoutTest+5*averCounts)
    {
        l1=static_cast<long>(averADC_V);
        qDebug()<<"l1"<<l1;

    }
    if (T==warmingTime+35*timeoutTest+5*averCounts)
    {
        l2=static_cast<long>(ADC_V);
        qDebug()<<"l2"<<l2;
        C2_3=(l0*(35*timeoutTest-29*timeoutTest)/((l2-l1)*R3));
        qDebug()<<"C2_3"<<C2_3;
    }
    //************************************************* Четвертая иттерация *****************
    if (T==warmingTime+36*timeoutTest+5*averCounts)
    {
        emit sendToCom(h61);
        delay(10000);
        emit sendToCom(h61);// сброс заряда
    }
    if (T==warmingTime+37*timeoutTest+5*averCounts)
    {
        ui->progressBar->setValue(80);
    }
    if (T==warmingTime+37*timeoutTest+5*averCounts)
    {
        emit sendToCom(h45);
        delay(10000);
        emit sendToCom(h45);// зарядка конденсатора
    }
    if (T==warmingTime+38*timeoutTest+5*averCounts)
    {
        l1=static_cast<long>(averADC_V);
        qDebug()<<"l1"<<l1;

    }
    if (T==warmingTime+44*timeoutTest+5*averCounts)
    {
        l2=static_cast<long>(ADC_V);
        qDebug()<<"l2"<<l2;
        C2_4=(l0*(44*timeoutTest-38*timeoutTest)/((l2-l1)*R3));
        qDebug()<<"C2_4"<<C2_4;
        C2=1000*(C2_1+C2_2+C2_3+C2_4)/4;
        qDebug()<<"C2"<<C2;

        QFile cal("/home/pi/calibration.txt");
        QStringList strList;
        if(cal.exists()&&cal.open(QIODevice::ReadOnly))
        {
            while(!cal.atEnd())
            {
                strList.append(cal.readLine());
            }
            cal.close();
        }
        strList[3]=QString::number(C2,'d',0)+"\n";

        if(cal.exists()&&cal.open(QIODevice::WriteOnly))
        {
            QTextStream stream(&cal);
            foreach(QString str,strList)
            {
                stream<<str;
            }
            cal.close();
        }
    }
    if (T==warmingTime+46*timeoutTest+5*averCounts)
    {
        emit sendToCom(h61);
        delay(10000);
        emit sendToCom(h61);// сброс заряда
        ui->progressBar->setValue(100);
    }

    //*************************************** Измерение тока ********************************************

    if (T==warmingTime+47*timeoutTest+5*averCounts)
    {
        ui->progressBar->setValue(0);
        ui->label_6->setText("Входной ток");
        emit sendToCom(h03);
        delay(10000);
        emit sendToCom(h03);// К1+К9
    }
    if (T==warmingTime+48*timeoutTest+5*averCounts)
    {
        ui->progressBar->setValue(5);
        ui->label_6->setText("Входной ток");
    }
    if (T==warmingTime+49*timeoutTest+5*averCounts)
    {
        ui->progressBar->setValue(10);
    }
    if (T==warmingTime+50*timeoutTest+5*averCounts)
    {
        ui->progressBar->setValue(15);
    }
    if (T==warmingTime+51*timeoutTest+5*averCounts)
    {
        ui->progressBar->setValue(20);
    }
    if (T==warmingTime+52*timeoutTest+5*averCounts)
    {
        ui->progressBar->setValue(25);
    }
    if (T==warmingTime+53*timeoutTest+5*averCounts)
    {
        ui->progressBar->setValue(30);
    }
    if (T>=warmingTime+53*timeoutTest+5*averCounts && T<=warmingTime+53*timeoutTest+6*averCounts)
    {
        ui->progressBar->setValue(40+10*(T-(warmingTime+53*timeoutTest+5*averCounts)));
        y+=averADC_V;
        if(T==warmingTime+53*timeoutTest+6*averCounts)
        {
            l1=y/(averCounts+1); // зреднее значение АЦП в разрядах
            U_Ioff=(l1-l0);
            I=(U_Ioff*0.0000003*1e-9)/R3;
            qDebug()<<"I"<<I;
            y=0;

            QFile cal("/home/pi/calibration.txt");
            QStringList strList;
            if(cal.exists()&&cal.open(QIODevice::ReadOnly))
            {
                while(!cal.atEnd())
                {
                    strList.append(cal.readLine());
                }
                cal.close();
            }
            strList[4]=QString::number(U_Ioff)+"\n";

            if(cal.exists()&&cal.open(QIODevice::WriteOnly))
            {
                QTextStream stream(&cal);
                foreach(QString str,strList)
                {
                    stream<<str;
                }
                cal.close();
            }
            ui->progressBar->setValue(100);
            emit testEnd();
            close();
            delete this;
        }
    }
}

//*****************************************************************************************
void TestCom::timeout_timerADC() // функция таймаута таймера измерения ADC
{
    ui->label_5->setText(QString::number(T_ADC));
    T_ADC+=1;
    x+=ADC_V;
    if(T_ADC%averNum==0)
    {
        averADC_V=x/averNum; // зреднее значение АЦП в разрядах
        x=0;
    }
    ui->label_3->setText(QString::number((averADC_V-l0)*0.0000003,'f',7));
}
//*****************************************************************************************

void TestCom::on_pushButton_clicked() //кнопка назад
{
    close();
    emit closePortTest();
    delete this;
}
void TestCom::recieveADCdec(QString str) // функция получения значения АЦП
{
    ui->label->setText(str);
    ADC_V=str.toDouble();
    ui->label_2->setText((QString::number((ADC_V-l0)*0.0000003,'f')));
}
