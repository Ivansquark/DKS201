#include "measuring_popup.h"
#include "ui_measuring_popup.h"

Measuring_Popup::Measuring_Popup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Measuring_Popup)
{
    ui->setupUi(this);
    //DACmax=33;
    //ui->label_18->setText(QString::number(DACmax));
    QFile test("/home/pi/test.txt");
    QStringList sList;
    if (test.exists()&&test.open(QIODevice::ReadOnly))
    {
        while(!test.atEnd())
        {
            sList.append(test.readLine());
        }
        test.close();
    }
    DACmin=sList[0].toInt();
    DACmax=sList[1].toInt();
    U_Offset_min=sList[2].toDouble();
    U_Offset_max=sList[3].toDouble();
    R2min=sList[4].toDouble();
    R2max=sList[5].toDouble();
    R3min=sList[6].toDouble();
    R3max=sList[7].toDouble();
    C2min=sList[8].toDouble();
    C2max=sList[9].toDouble();
    Imin=sList[10].toDouble();
    Imax=sList[11].toDouble();

    QFile config("/home/pi/config.txt");
    QStringList strList;
    if (config.exists()&&config.open(QIODevice::ReadOnly))
    {
        while(!config.atEnd())
        {
            strList.append(config.readLine());
        }
        config.close();
    }
    ui->label_5->setText(strList[1]);
    ui->label_11->setText(strList[2]);
    ui->label_15->setText(strList[3]);

    QFile cal("/home/pi/calibration.txt");
    QStringList strList1;
    if (cal.exists()&&cal.open(QIODevice::ReadOnly))
    {
        while(!cal.atEnd())
        {
            strList1.append(cal.readLine());
        }
        cal.close();
    }
    double R3 = strList1[2].toDouble();
    ui->label_3->setText(QString::number(strList1[0].toDouble()*1e6));
    ui->label_7->setText(strList1[1]);
    ui->label_9->setText(strList1[2]);
    ui->label_13->setText(strList1[3]);
    ui->label_17->setText(QString::number(((strList1[4].toDouble()*0.0003)/R3),'d',3));

    QString error="";

    if (!((strList1[5].toInt()>=DACmin)&&(strList1[5].toInt()<=DACmax))) error.append("Напряжение на ЦАП вышло за пределы\n");
    if (!((strList1[0].toDouble()>=U_Offset_min)&&(strList1[0].toDouble()<=U_Offset_max))) error.append("Напряжение смещения вышло за пределы\n");
    if (!((strList1[1].toDouble()>=R2min)&&(strList1[1].toDouble()<=R2max))) error.append("Значение R2 вышло за пределы\n");
    if (!((strList1[2].toDouble()>=R3min)&&(strList1[2].toDouble()<=R3max))) error.append("Значение R3 вышло за пределы\n");
    if (!((strList1[3].toDouble()>=C2min)&&(strList1[3].toDouble()<=C2max))) error.append("Значение C2+C1 вышло за пределы\n");
    if (!(((strList1[4].toDouble()*0.0003)/R3>=Imin)&&(strList1[4].toDouble()<=Imax))) error.append("Значение входного тока вышло за пределы");
    ui->label_18->setText(error);
}

Measuring_Popup::~Measuring_Popup()
{
    delete ui;
}

void Measuring_Popup::on_pushButton_3_clicked() // кнопка отмена
{
    close();
    delete this;
}

void Measuring_Popup::on_pushButton_2_clicked() // кнопка повторить тест
{
    close();
    emit repeatTest();
    delete this;
}

void Measuring_Popup::on_pushButton_clicked() // кнопка ок - при корректных результатах теста позволяет программе работать далее
{
    close();
    emit okTest();
    delete this;
}

void Measuring_Popup::on_pushButton_4_clicked() //кнопка записать эталонные (только для эмулятора);
{
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
    strList[0]="0.000001\n";
    strList[1]="100.001\n";
    strList[2]="10.001\n";
    strList[3]="47000.001\n";
    strList[4]="0.000002\n";
    ui->label_3->setText(strList[0]);
    ui->label_7->setText(strList[1]);
    ui->label_9->setText(strList[2]);
    ui->label_13->setText(strList[3]);
    ui->label_17->setText(strList[4]);

    QFile cal1("/home/pi/calibration.txt");
    //QStringList strList1;
    QTextStream stream(&cal);
    if(cal.exists()&&cal.open(QIODevice::WriteOnly))
    {
        for (QString str:strList)
        {
            stream<<str;
        }
        cal.close();
    }
    //ui->label_18->setText("Все стало хорошо");
}
