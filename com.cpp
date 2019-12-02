//********************** Класс СОМ порта считывает данные в QString и передает QByteArray
#include "com.h"
#include <QDebug>

Com::Com(QObject *parent):QObject(parent)
{
    //****************************** Настройки ************************
    port->setPortName("/dev/ttyUSB0");
    port->setBaudRate(QSerialPort::Baud2400);
    port->setDataBits(QSerialPort::Data8);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->setFlowControl(QSerialPort::NoFlowControl);

    //**************************** Коды реле **************************

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

    qDebug()<<"Constr";
}
Com::~Com()
{
    delete timer1;
    delete timerDelay;
    qDebug()<<"Destr Port";
}
void Com::closePort()
{
    qDebug()<<"delete Port";
    delete this;
}
void Com :: process_Port()
{   //Выполняется при старте потока
    qDebug()<<"process_Port";
    connect(this, SIGNAL(PortConnect()),this,SLOT(ReadInPort())); //подключаем   чтение с порта по сигналу connect
    emit process_Port_Start();
}
void Com::timeOutDelay()
{
    qDebug()<<"timeOutDelay";
    connect(timer1,SIGNAL(timeout()), this, SLOT(timeOut())); // подключаем timer
    timer1->start(100); // обновление информации с СОМ порта
}

void Com :: ConnectPort(void)
{//процедура подключения
    qDebug()<<"ConnectPort";
    if (port->open(QIODevice::ReadWrite))
    {
       if (port->isOpen())
       {
           qDebug()<<" Открыт!";
           port->setDataTerminalReady(true);
           port->setRequestToSend(true);
           emit PortConnect();
           qDebug()<<" PortConnect!";
       }
       else
       {
           port->close();
       }
    }
    else
    {
        port->close();
    }
}

void  Com::DisconnectPort()
{//Отключаем порт
    if(port->isOpen())
    {
        port->close();
        qDebug()<<"Закрыт!";
        emit finished_Port();
    }
    //delete this;
}
void Com :: WriteToPort(QByteArray data)
{//Запись данных в порт
    if(port->isOpen())
    {
        port->clear();//- очищаем порт
        port->write(data);
        qDebug()<<QString::number(data[0],16)<<
                  QString::number(data[1],16)<<
                  QString::number(data[2],16)<<
                  QString::number(data[3],16)<<
                  QString::number(data[4],16)<<
                  QString::number(data[5],16)<<
                  QString::number(data[6],16);
    }
}

void Com :: ReadInPort()
{//Чтение данных из порта
    qDebug()<<"ReadInPort";
    connect(timerDelay,SIGNAL(timeout()),this,SLOT(timeOutDelay()));
    timerDelay->setSingleShot(true);
    timerDelay->start(6000);
}
void Com::timeOut()
{
    if(port->isOpen())
    {
        long long i = port->QSerialPort::bytesAvailable();
        QByteArray arrList;
        if(i>=7)
        {
            arrList.append((port->read(6)));

             int *arr = new int [6];
             for(int i=0;i<6;i++)
             {
                 arr[i]=arrList[i];
             }
             std::sort(&arr[0],&arr[6]); // функция сортировки байтов по порядку (при считывании иногда теряется порядок!!!)

             if(arr[0]!=arr[1]&&arr[0]!=arr[2]&&arr[0]!=arr[3]&&arr[0]!=arr[4]&&arr[0]!=arr[5]
                     &&arr[1]!=arr[2]&&arr[1]!=arr[2]&&arr[1]!=arr[3]&&arr[1]!=arr[4]&&arr[1]!=arr[5]
                     &&arr[2]!=arr[3]&&arr[2]!=arr[4]&&arr[2]!=arr[5]
                     &&arr[3]!=arr[4]&&arr[3]!=arr[5]
                     &&arr[4]!=arr[5]) // условие неравенства байтов между собой (иногда пропускались байты и было несколько одинаковых!)
             {
                 COM=QString::number(arr[0],16)+QString::number(arr[1],16)+
                         QString::number(arr[2],16)+QString::number(arr[3],16)+
                         QString::number(arr[4],16)+QString::number(arr[5],16);
             }

             delete [] arr;
        }
       emit outPort(COM);
       //qDebug()<<COM;
       port->clear(); // очищаем очередь порта
    }    
}




