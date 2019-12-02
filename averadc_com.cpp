#include "averadc_com.h"

AverADC_COM::AverADC_COM()
{

}
AverADC_COM::~AverADC_COM()
{
    qDebug()<<"destruptor averADC";
    emit close1();
}
void AverADC_COM::recieveCOM(QString str) // принимает данные СОМ порта и передает их дальше в MainWindow
{
    COM=str;
    emit sendCOM(COM);
    //qDebug()<<COM;

    const char* charCOM = COM.toStdString().c_str();// - перевод строки в массив символов;
    QString strADC_Hex="";
    for (int i=1;i<COM.length();i+=2) // - перевод массива символов в строку hex кода ADC;
    {
        strADC_Hex.append(charCOM[i]);
    }
    //qDebug()<<strADC_Hex;
    long numADC=strADC_Hex.toLong(nullptr,16); //-перевод строки в десятичное число;
    emit sendADC(QString::number(numADC)); // - посылка значения ADС в форме десятичной строки
    //qDebug()<<numADC;
}
void AverADC_COM::close2()
{
    close();
    delete this;
}
