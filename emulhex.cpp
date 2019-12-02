#include "emulhex.h"

EmulHex::EmulHex()
{


}
EmulHex::~EmulHex()
{
    //delete this;
    qDebug()<<"destruptor EmulHex";
}
void EmulHex::timeOut() // функция по окончанию времени таймера увеличивает вторые символы в строке 273f4f5f6f7f
{

    const char *symbols=com.toStdString().c_str(); // переводит QString в массив символов char.

    QString str;
    for (int j=1;j<com.length();j+=2)
    {
        str.append(symbols[j]);
    }
    long numADC=str.toLong(nullptr,16)+iter; // переводит шестнадцатеричную строку в число long + его увеличение по таймеру;
    iter++;
    QString B_16=QString("%1").arg(numADC,0,16); // переводит число в шестнадцатиричную строку одной функцией, а не через самостоятельный цикл;

    const char *symbols1 = B_16.toStdString().c_str();
    QString strB_16="";
    for(int j=0;j<B_16.length();j++) // преобразует 7ffffff в 273f4f5f6f7f
    {
        strB_16.append(QString::number(j+2));
        strB_16.append(symbols1[j]);
    }
    sendcom=strB_16;

    //qDebug()<<sendcom;
    emit sendCOM(sendcom);
    //delete [] symbols;
}

void EmulHex::close1()
{
    close();
    destroy();
    delete this;         // удаляет данный объект из кучи (чтобы не считал далее по таймеру)
    //qDebug()<<"opachki";
}

void EmulHex::recieveEMUL()
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
        AverageADC *avrADC;
        avrADC=new AverageADC;
        avrADC->setModal(false);
        connect(timer,SIGNAL(timeout()),this,SLOT(timeOut()));
        connect(this,SIGNAL(sendEmulBegin()),avrADC,SLOT(recieveEmulBegin()));
        connect(this,SIGNAL(sendCOM(QString)),avrADC,SLOT(recieveCOM(QString)));
        timer->start(1); // - 1 мс
        connect(avrADC,SIGNAL(close1()),this,SLOT(close1()));
        connect(avrADC,SIGNAL(sendTestOff2()),this,SLOT(sendTestOff3()));
        emit sendEmulBegin();
    }
}
void EmulHex::sendTestOff3()
{
    emit sendTestOff4();
}

void EmulHex::recieveMesBegin()
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
        AverageADC *avrADC;
        avrADC= new AverageADC;
        avrADC->setModal(false);
        connect(timer,SIGNAL(timeout()),this,SLOT(timeOut()));
        connect(this,SIGNAL(sendMesBegin()),avrADC,SLOT(recieveMesBegin()));
        connect(this,SIGNAL(sendCOM(QString)),avrADC,SLOT(recieveCOM(QString)));
        timer->start(1); // - 1 мс
        connect(avrADC,SIGNAL(close1()),this,SLOT(close1()));
        connect(avrADC,SIGNAL(sendTestOff2()),this,SLOT(sendTestOff3()));
        emit sendMesBegin();
    }
}
