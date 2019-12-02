#include "connect_com.h"
#include "ui_connect_com.h"
#include <QTime>

Connect_com::Connect_com(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Connect_com)
{
    ui->setupUi(this);


    PortNew->moveToThread(thread_New);//помешаем класс  в поток
    PortNew->port->moveToThread(thread_New);//Помещаем сам порт в поток

        connect(thread_New, SIGNAL(started()), PortNew, SLOT(process_Port()));//Переназначения метода run !!!!!!!!
        connect(PortNew, SIGNAL(finished_Port()), thread_New, SLOT(quit()));//Переназначение метода выход
        connect(ui->pushButton_10, SIGNAL(clicked()),PortNew,SLOT(ConnectPort()));//по нажатию кнопки подключить порт
        connect(ui->pushButton_11, SIGNAL(clicked()),PortNew,SLOT(DisconnectPort()));//по нажатию кнопки отключить порт
        connect(PortNew, SIGNAL(outPort(QString)), this, SLOT(print(QString)));//вывод в текстовое поле считанных данных
        connect(this,SIGNAL(writeData(QByteArray)),PortNew,SLOT(WriteToPort(QByteArray)));//отправка в порт данных


}

Connect_com::~Connect_com()
{
    delete ui;    
}

void Connect_com::SortCOM(QSerialPort *serialPort)
{
    if(serialPort->isOpen())
    {
        long long i = serialPort->QSerialPort::bytesAvailable();
        QByteArray arrList;
        if(i>=7)
        {
            arrList.append((serialPort->read(6)));

                 int *arr = new int [6];
                 for(int i=0;i<6;i++)
                 {
                     arr[i]=arrList[i];
                 }
                 std::sort(&arr[0],&arr[6]);
                 COM=QString::number(arr[0],16)+QString::number(arr[1],16)+
                         QString::number(arr[2],16)+QString::number(arr[3],16)+
                         QString::number(arr[4],16)+QString::number(arr[5],16);
                 delete [] arr;
        }
    }
}

void Connect_com::on_pushButton_clicked() //close
{
    close();
    delete thread_New;
}

void Connect_com::on_pushButton_7_clicked()// thread
{   
       thread_New->start();
            //PortNew->ReadInPort();
}
void Connect_com::print(QString COM)
{
    qDebug()<<COM;
    ui->lineEdit_3->setText(COM);
}

void Connect_com::on_pushButton_8_clicked()//thread terminate
{

}

void Connect_com::on_pushButton_9_clicked()//Stop_Read
{
    emit timerStop();
    while(current_serial->bytesAvailable())
    {
        current_serial->readAll();
        int i=0;
        i++;
        qDebug()<<i;
    }
    current_serial->clear();
}

void Connect_com::on_pushButton_12_clicked() //threadRead
{
    thread_New->terminate();
}

void Connect_com::on_pushButton_2_clicked()
{
    qDebug()<<PortNew->h00;
    qDebug()<<PortNew->h00[0]<<PortNew->h00[1];
    qDebug()<<QString::number(PortNew->h00[0],16)<<QString::number(PortNew->h00[1],16);
    qDebug()<<QString::number(PortNew->h21[0],16)<<QString::number(PortNew->h21[1],16);

}

void Connect_com::on_pushButton_3_clicked() //c2
{
    PortNew->WriteToPort(PortNew->h45);
}

void Connect_com::on_pushButton_4_clicked() // c2=0
{
    PortNew->WriteToPort(PortNew->h61);
}

void Connect_com::on_pushButton_10_clicked()
{

}
