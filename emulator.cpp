#include "emulator.h"
#include "ui_emulator.h"

Emulator::Emulator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Emulator)
{
    ui->setupUi(this);
    QFile advconfig("/home/pi/advconfig.txt");
    QStringList strList1;
    if(advconfig.exists()&&advconfig.open(QIODevice::ReadOnly))
    {
        while(!advconfig.atEnd())
        {
            strList1.append(advconfig.readLine());
        }
        advconfig.close();
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



    QString time=QTime::currentTime().toString();
    ui->label->setText(time);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerAlarm()));
    connect(timer, SIGNAL(timeout()), this, SLOT(sredneeADC_V()));

    timer->start(1000); // И запустим таймер
    ui->label_2->setText("0");

    ui->lineEdit_2->setText("0");
    ui->lineEdit_3->setText("0");
    ui->label_11->setText("0");
    ui->progressBar->setValue(0);

}

Emulator::~Emulator()
{
    delete ui;
    qDebug()<<"destruptorEmulator";
    emit close2();
}
void Emulator::slotTimerAlarm() // - Референсный таймер
{
    ui->label->setText(QTime::currentTime().toString());
    timeREF+=1;
    QString str="",str1="";
    str1=str.number(timeREF);
    ui->label_11->setText(str1);
}


void Emulator::recieveADC(QString str) // - функция срабатываемая по принятию данных ADC (время срабатывания зависит от времени опроса порта)
{
    ui->label_14->setText(str);
    strADC=str;
        QString str1="",str2="";
        str2=str1.number(i);
        ui->label_2->setText(str2);
        i++; // - увеличиваем счетчик количества отсчетов

        x=x+strADC.toLong(); // - последовательно складываем значения с АЦП в десятичной форме
        if (i%averNum==0) // - если остаток от деления на Количество отсчетов необходимого для вычисления среднего значения АЦП
        {
            y=x/averNum; //- присваиваем y среднее значение показаний считанных с АЦП
            x=0; // - обнуляем сумму АЦП в десятичной форме (считаем заново)
        }
        QString strADC1="";
        strADC1=str.number(y);
        ui->lineEdit->setText(strADC1);

        ADC_V=(y-8388607)*0.0000003;
        QString strADC_V="",str3="";
        strADC_V=str3.number(ADC_V,'d',7);
        if (ADC_V>0)
        {
        ui->lineEdit_2->setText(strADC_V);
        }
}
void Emulator::sredneeADC_V() // функция срабатывает по таймеру (timer) на 1000 мс
{
    //********************ЭМУЛЯТОР*************************************

    // Определение кода ЦАП и измерение напряжения смещения
    if ((timeREF >0)&&(timeREF <warmingTime))
    {
        ui->label_13->setText("Прогрев оборудования");
            if (timeREF==warmingTime/2) ui->progressBar->setValue(0);

    }
            if (timeREF==warmingTime+1) ui->progressBar->setValue(33);
            if (timeREF==warmingTime+2) ui->progressBar->setValue(66);
            if (timeREF==warmingTime+3) ui->progressBar->setValue(100);

    if (timeREF ==warmingTime+4)
    {
             ui->progressBar->setValue(0);
        l1=y;
        ui->lineEdit_3->setText("0");
        ui->label_10->setText(QString::number(l1));
        ui->label_13->setText("Определение кода ЦАП");
    }
            if (timeREF==warmingTime+5) ui->progressBar->setValue(33);
            if (timeREF==warmingTime+6) ui->progressBar->setValue(66);

    if (timeREF==warmingTime+7)
    {
            ui->progressBar->setValue(100);
        l2=y;
        ui->label_16->setText(QString::number(l2));
        ui->lineEdit_3->setText("4095");
    }
    if (timeREF==warmingTime+8)
    {

        d=abs((static_cast<double>(l0)-static_cast<double>(l1))/(static_cast<double>(l2)-static_cast<double>(l1)));

        DAC=4095*d;
        long DAC1=static_cast<long>(DAC);
        ui->label_17->setText(QString::number(DAC1,16));
        ui->lineEdit_3->setText(QString::number(DAC,'f',0));
        ui->progressBar->setValue(100);
    }

    if ((timeREF >warmingTime+8)&&(timeREF <warmingTime+10))
    {
        ui->label_13->setText("Измерение напряжения смещения");
        if (timeREF == warmingTime+9)
        {
             ui->progressBar->setValue(0);
            ui->progressBar->setValue(50);
            ui->lineEdit_8->setText(QString::number(ADC_V,'d',7));
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
            strList[0]=QString::number(ADC_V,'d',7)+"\n";

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
    }
            if (timeREF==warmingTime+10) ui->progressBar->setValue(100);

    if ((timeREF >warmingTime+10)&&(timeREF <warmingTime+10+timeoutTEST))
    {
        ui->label_13->setText("Таймаут");
    }
            if (timeREF==warmingTime+10+timeoutTEST) ui->progressBar->setValue(0);

    if ((timeREF >warmingTime+10+timeoutTest)&&(timeREF < warmingTime+10+timeoutTest+3))
    {

        ui->label_13->setText("Калибровка R2");
        if (timeREF==warmingTime+10+timeoutTest+2)
        {
            ui->progressBar->setValue(30);
                if (timeREF==warmingTime+10+timeoutTest+2) ui->progressBar->setValue(66);
            l1=y;
            R2=((l0*R1*1000000)/(l1-l0) - 100000)/1000000;   // - калибровка R2, в МОм
            //ui->label_16->setText(QString::number(l2));
            ui->label_12->setText(QString::number(R1));
            ui->lineEdit_4->setText(QString::number(R2,'d',2));

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
            }
        }
    }
            if (timeREF==warmingTime+10+timeoutTest+3)  ui->progressBar->setValue(100);

    if ((timeREF >warmingTime+10+timeoutTest+3)&&(timeREF <warmingTime+10+timeoutTest+3+timeoutTest))
    {
        ui->label_13->setText("Таймаут");
    }
            if (timeREF==warmingTime+10+timeoutTest+3+timeoutTest)  ui->progressBar->setValue(0);

    if ((timeREF >warmingTime+10+timeoutTest+3+timeoutTest)&&(timeREF < warmingTime+10+timeoutTest+3+timeoutTest+4))
    {
        ui->label_13->setText("Калибровка R3");
                if (timeREF==warmingTime+10+timeoutTest+3+timeoutTest+1)  ui->progressBar->setValue(33);
                if (timeREF==warmingTime+10+timeoutTest+3+timeoutTest+2)  ui->progressBar->setValue(66);
        if (timeREF==warmingTime+10+timeoutTest+3+timeoutTest+3)
        {
            l1=y;
            R3=((l0*R2*1000000)/(l1-l0) - 100000)/1000000000; // - калибровка R3, в ГОм
            //ui->label_16->setText(QString::number(l2));
            ui->label_12->setText(QString::number(R1));
            ui->lineEdit_5->setText(QString::number(R3,'d',2));

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
            }
        }
    }
                if (timeREF==warmingTime+10+timeoutTest+3+timeoutTest+4)  ui->progressBar->setValue(100);


    if ((timeREF >warmingTime+10+timeoutTest+3+timeoutTest+4)&&(timeREF < warmingTime+10+timeoutTest+3+timeoutTest+4+timeoutTest))
    {
        ui->label_13->setText("Таймаут");
    }

    if ((timeREF >warmingTime+10+timeoutTest+3+timeoutTest+4+timeoutTest)&&(timeREF < warmingTime+10+timeoutTest+3+timeoutTest+4+timeoutTest+5))
    {
        ui->progressBar->setValue(0);
        ui->label_13->setText("Калибровка С2");
        if (timeREF==warmingTime+10+timeoutTest+3+timeoutTest+4+timeoutTest+1)
        {
            ui->progressBar->setValue(25);
            l1=y;
        }
                if (timeREF==warmingTime+10+timeoutTest+3+timeoutTest+4+timeoutTest+2)  ui->progressBar->setValue(50);
                if (timeREF==warmingTime+10+timeoutTest+3+timeoutTest+4+timeoutTest+3)  ui->progressBar->setValue(75);
        if (timeREF==warmingTime+10+timeoutTest+3+timeoutTest+4+timeoutTest+4)
        {
            l2=y;
            C2=((l0*((warmingTime+10+timeoutTest+3+timeoutTest+4+timeoutTest+4)-warmingTime+10+timeoutTest+3+timeoutTest+4+timeoutTest+1)))
                    /((l2-l1)*R3); // - калибровка C2, в пФ
            ui->lineEdit_6->setText(QString::number(C2,'d',2));

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
            strList[3]=QString::number(C2,'d',4)+"\n";

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
    }

            if (timeREF==warmingTime+10+timeoutTest+3+timeoutTest+4+timeoutTest+5)  ui->progressBar->setValue(100);

    if ((timeREF >warmingTime+10+timeoutTest+3+timeoutTest+4+timeoutTest+5)&&
            (timeREF < warmingTime+10+timeoutTest+3+timeoutTest+4+timeoutTest+5+timeoutTEST))
    {
        ui->label_13->setText("Таймаут");
    }
    if (timeREF ==warmingTime+10+timeoutTest+3+timeoutTest+4+timeoutTest+5+timeoutTEST) ui->progressBar->setValue(0);

    if ((timeREF >warmingTime+10+timeoutTest+3+timeoutTest+4+timeoutTest+5+timeoutTEST)&&
            (timeREF < warmingTime+10+timeoutTest+3+timeoutTest+4+timeoutTest+5+timeoutTEST+2)) // - измерение входного тока
    {

        ui->label_13->setText("Измерение входного тока");


        if (timeREF == warmingTime+10+timeoutTest+3+timeoutTest+4+timeoutTest+5+timeoutTEST+1)
        {
            ui->progressBar->setValue(50);
            l1=y;

            I=(l1/R3); // - измерение входного тока------------------------------
            //ui->label_16->setText(QString::number(l2));
            ui->label_12->setText(QString::number(R1));
            ui->lineEdit_7->setText(QString::number(I,'d',2));

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
            strList[4]=QString::number(I,'d',3)+"\n";

            if(cal.exists()&&cal.open(QIODevice::WriteOnly))
            {
                QTextStream stream(&cal);
                foreach(QString str,strList)
                {
                    stream<<str;
                }
                cal.close();
            }

            QStringList strList1;
            if(cal.exists()&&cal.open(QIODevice::ReadOnly))
            {
                while(!cal.atEnd())
                {
                    strList1.append(cal.readLine());
                }
                cal.close();
            }
            strList[5]=QString::number(DAC,'d',0)+"\n";

            if(cal.exists()&&cal.open(QIODevice::WriteOnly))
            {
                QTextStream stream(&cal);
                foreach(QString str,strList1)
                {
                    stream<<str;
                }
                cal.close();
            }
        }
    }
    if (timeREF==warmingTime+10+timeoutTest+3+timeoutTest+4+timeoutTest+5+timeoutTEST+2)  ui->progressBar->setValue(100);

    if (timeREF==warmingTime+10+timeoutTest+3+timeoutTest+4+timeoutTest+5+timeoutTEST+4)
    {
        close();
        emit signalTestOff();
        delete this;
    }
    //**************************ПОСЛЕДОВАТЕЛЬНЫЙ ПОРТ***********************************
    //
    //
}
void Emulator::recieveCOM(QString str)
{
    ui->label_15->setText(str);
}


void Emulator::on_pushButton_clicked()
{
    close();
    delete this;
}
