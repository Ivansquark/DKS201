// класс - принимает или строку из эмулятора или массив байтов из последовательного порта
//переводит данные в десятичный код АЦП, вычисляет среднее и передает далее в программу измерения;

#include "averageadc.h"

AverageADC::AverageADC()
{
    QFile advconfig("/home/pi/advconfig.txt");
    QStringList strList;
    if (advconfig.exists()&&advconfig.open(QIODevice::ReadOnly))
    {
        while(!advconfig.atEnd())
        {
            strList.append(advconfig.readLine());
        }
        advconfig.close();
    }
    requestPeriod=static_cast<int>(strList[5].toDouble()*1000); // - задание периода опроса в мс для таймера отправки
    timer = new QTimer(this);
    timer->start(requestPeriod);
    connect(timer,SIGNAL(timeout()),this,SLOT(TimeOut()));
}

AverageADC::~AverageADC()
{

    qDebug()<<"destruptor averADC";
    emit close1();
}


void AverageADC::recieveCOM(QString str) // принимает данные СОМ порта и передает их дальше в тест
{
    COM=str;
    emit sendCOM(COM);  

    //**************************************ЕСЛИ РАБОТАЕТ COM***************************************

    const char* charCOM = COM.toStdString().c_str();// - перевод строки в массив символов;
    QString strADC_Hex="";
    for (int i=1;i<COM.length();i+=2) // - перевод массива символов в строку hex кода ADC;
    {
        strADC_Hex.append(charCOM[i]);
    }
    long numADC=strADC_Hex.toLong(nullptr,16); //-перевод строки в десятичное число;

    emit sendADC(QString::number(numADC)); // - посылка значения ADС в форме десятичной строки
    //qDebug()<<numADC;
}


//**************************************ЕСЛИ РАБОТАЕТ ЭМУЛЯТОР***************************************
void AverageADC::TimeOut() // - время срабатывания функции задается параметром - период опроса порта (для эмулятора)
{
    const char* charCOM = COM.toStdString().c_str();// - перевод строки в массив символов;
    QString strADC_Hex="";
    for (int i=1;i<COM.length();i+=2) // - перевод массива символов в строку hex кода ADC;
    {
        strADC_Hex.append(charCOM[i]);
    }
    long numADC=strADC_Hex.toLong(nullptr,16); //-перевод строки в десятичное число;

    emit sendADC(QString::number(numADC)); // - посылка значения ADС в форме десятичной строки
    //qDebug()<<numADC;

}
void AverageADC::close2()
{
    close();
    delete this;
}
void AverageADC::signalTestOff1()
{
    emit sendTestOff2();
}
void AverageADC::recieveEmulBegin()
{
    QString str;
    QFile config("/home/pi/config.txt"); //- опрос по режиму работы - эмулятор или последовательный порт
    if (config.exists()&&config.open(QIODevice::ReadOnly))
    {
        str.append(config.readLine());
        config.close();
    }
    if (str=="2\n") // - подключен эмулятор
    {
        test = new Emulator();
        test->setModal(true);
        test->showFullScreen();
        connect(this,SIGNAL(sendADC(QString)),test,SLOT(recieveADC(QString)));
        connect(this,SIGNAL(sendCOM(QString)),test,SLOT(recieveCOM(QString)));
        connect(test,SIGNAL(close2()),this,SLOT(close2()));
        connect(test,SIGNAL(signalTestOff()),this,SLOT(signalTestOff1()));
    }
    if (str=="1\n") // - подключен COM
    {
        TestCom *test1= new TestCom();
        test1->setModal(true);
        test1->showFullScreen();
        connect(this,SIGNAL(sendADC(QString)),test1,SLOT(recieveADC(QString)));
        connect(this,SIGNAL(sendCOM(QString)),test1,SLOT(recieveCOM(QString)));
        connect(test1,SIGNAL(close2()),this,SLOT(close2()));
        connect(test1,SIGNAL(signalTestOff()),this,SLOT(signalTestOff1()));
    }
}
void AverageADC::recieveMesBegin()
{
    QString str;
    QFile config("/home/pi/config.txt"); //- опрос по режиму работы - эмулятор или последовательный порт
    if (config.exists()&&config.open(QIODevice::ReadOnly))
    {
        str.append(config.readLine());
        config.close();
    }
    if (str=="2\n") // - подключен эмулятор
    {
        Measuring *mes = new Measuring;
        mes->setModal(true);
        mes->showFullScreen();
        connect(this,SIGNAL(sendADC(QString)),mes,SLOT(recieveADC(QString)));
        connect(mes,SIGNAL(close2()),this,SLOT(close2()));
    }
    if (str=="1\n") // - подключен СОМ
    {
        Measuring *mes = new Measuring;
        mes->setModal(true);
        mes->showFullScreen();
        connect(this,SIGNAL(sendADC(QString)),mes,SLOT(recieveADC(QString)));
        connect(mes,SIGNAL(close2()),this,SLOT(close2()));
    }
}
