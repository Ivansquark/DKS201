#include "setup_test.h"
#include "ui_setup_test.h"

Setup_Test::Setup_Test(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setup_Test)
{
    ui->setupUi(this);
    QFile test("/home/pi/test.txt");
    QStringList strList;
    if (test.exists()&&test.open(QIODevice::ReadOnly))
    {
        while(!test.atEnd())
        {
            strList.append(test.readLine());
        }
        test.close();
    }
    QString str1=strList[0];
    str1.chop(1);
    QString str2=strList[1];
    str2.chop(1);
    QString str3=strList[2];
    str3.chop(1);
    QString str4=strList[3];
    str4.chop(1);
    QString str5=strList[4];
    str5.chop(1);
    QString str6=strList[5];
    str6.chop(1);
    QString str7=strList[6];
    str7.chop(1);
    QString str8=strList[7];
    str8.chop(1);
    QString str9=strList[8];
    str9.chop(1);
    QString str10=strList[9];
    str10.chop(1);
    QString str11=strList[10];
    str11.chop(1);
    QString str12=strList[11];
    str12.chop(1);
    QString str13=strList[12];
    str13.chop(1);

    ui->lineEdit->setText(str1);
    ui->lineEdit_2->setText(str2);
    ui->lineEdit_3->setText(str3);
    ui->lineEdit_4->setText(str4);
    ui->lineEdit_5->setText(str5);
    ui->lineEdit_6->setText(str6);
    ui->lineEdit_7->setText(str7);
    ui->lineEdit_8->setText(str8);
    ui->lineEdit_9->setText(str9);
    ui->lineEdit_10->setText(str10);
    ui->lineEdit_11->setText(str11);
    ui->lineEdit_12->setText(str12);
    ui->lineEdit_13->setText(str13);
}

Setup_Test::~Setup_Test()
{
    delete ui;
}

void Setup_Test::on_pushButton_clicked()
{
    close();
}
void Setup_Test::setText1(QString str)
{
    ui->lineEdit->setText(str);
    QFile test("/home/pi/test.txt");
    QStringList strList;
    QTextStream stream(&test);
    if (test.exists()&&test.open(QIODevice::ReadOnly))
    {
        while (!test.atEnd())
        {
            strList.append(test.readLine());
        }
        test.close();
    }
    strList[0]=ui->lineEdit->text()+"\n";
    if (test.exists()&&test.open(QIODevice::WriteOnly))
    {
        for (QString str:strList)
        {
            stream<<str;
        }
        test.close();
    }
}
void Setup_Test::setText2(QString str)
{
    ui->lineEdit_2->setText(str);
    QFile test("/home/pi/test.txt");
    QStringList strList;
    QTextStream stream(&test);
    if (test.exists()&&test.open(QIODevice::ReadOnly))
    {
        while (!test.atEnd())
        {
            strList.append(test.readLine());
        }
        test.close();
    }
    strList[1]=ui->lineEdit_2->text()+"\n";
    if (test.exists()&&test.open(QIODevice::WriteOnly))
    {
        for (QString str:strList)
        {
            stream<<str;
        }
        test.close();
    }
}
void Setup_Test::setText3(QString str)
{
    ui->lineEdit_3->setText(str);
    QFile test("/home/pi/test.txt");
    QStringList strList;
    QTextStream stream(&test);
    if (test.exists()&&test.open(QIODevice::ReadOnly))
    {
        while (!test.atEnd())
        {
            strList.append(test.readLine());
        }
        test.close();
    }
    strList[2]=ui->lineEdit_3->text()+"\n";
    if (test.exists()&&test.open(QIODevice::WriteOnly))
    {
        for (QString str:strList)
        {
            stream<<str;
        }
        test.close();
    }
}
void Setup_Test::setText4(QString str)
{
    ui->lineEdit_4->setText(str);
    QFile test("/home/pi/test.txt");
    QStringList strList;
    QTextStream stream(&test);
    if (test.exists()&&test.open(QIODevice::ReadOnly))
    {
        while (!test.atEnd())
        {
            strList.append(test.readLine());
        }
        test.close();
    }
    strList[3]=ui->lineEdit_4->text()+"\n";
    if (test.exists()&&test.open(QIODevice::WriteOnly))
    {
        for (QString str:strList)
        {
            stream<<str;
        }
        test.close();
    }
}
void Setup_Test::setText5(QString str)
{
    ui->lineEdit_5->setText(str);
    QFile test("/home/pi/test.txt");
    QStringList strList;
    QTextStream stream(&test);
    if (test.exists()&&test.open(QIODevice::ReadOnly))
    {
        while (!test.atEnd())
        {
            strList.append(test.readLine());
        }
        test.close();
    }
    strList[4]=ui->lineEdit_5->text()+"\n";
    if (test.exists()&&test.open(QIODevice::WriteOnly))
    {
        for (QString str:strList)
        {
            stream<<str;
        }
        test.close();
    }
}
void Setup_Test::setText6(QString str)
{
    ui->lineEdit_6->setText(str);
    QFile test("/home/pi/test.txt");
    QStringList strList;
    QTextStream stream(&test);
    if (test.exists()&&test.open(QIODevice::ReadOnly))
    {
        while (!test.atEnd())
        {
            strList.append(test.readLine());
        }
        test.close();
    }
    strList[5]=ui->lineEdit_6->text()+"\n";
    if (test.exists()&&test.open(QIODevice::WriteOnly))
    {
        for (QString str:strList)
        {
            stream<<str;
        }
        test.close();
    }
}
void Setup_Test::setText7(QString str)
{
    ui->lineEdit_7->setText(str);
    QFile test("/home/pi/test.txt");
    QStringList strList;
    QTextStream stream(&test);
    if (test.exists()&&test.open(QIODevice::ReadOnly))
    {
        while (!test.atEnd())
        {
            strList.append(test.readLine());
        }
        test.close();
    }
    strList[6]=ui->lineEdit_7->text()+"\n";
    if (test.exists()&&test.open(QIODevice::WriteOnly))
    {
        for (QString str:strList)
        {
            stream<<str;
        }
        test.close();
    }
}
void Setup_Test::setText8(QString str)
{
    ui->lineEdit_8->setText(str);
    QFile test("/home/pi/test.txt");
    QStringList strList;
    QTextStream stream(&test);
    if (test.exists()&&test.open(QIODevice::ReadOnly))
    {
        while (!test.atEnd())
        {
            strList.append(test.readLine());
        }
        test.close();
    }
    strList[7]=ui->lineEdit_8->text()+"\n";
    if (test.exists()&&test.open(QIODevice::WriteOnly))
    {
        for (QString str:strList)
        {
            stream<<str;
        }
        test.close();
    }
}
void Setup_Test::setText9(QString str)
{
    ui->lineEdit_9->setText(str);
    QFile test("/home/pi/test.txt");
    QStringList strList;
    QTextStream stream(&test);
    if (test.exists()&&test.open(QIODevice::ReadOnly))
    {
        while (!test.atEnd())
        {
            strList.append(test.readLine());
        }
        test.close();
    }
    strList[8]=ui->lineEdit_9->text()+"\n";
    if (test.exists()&&test.open(QIODevice::WriteOnly))
    {
        for (QString str:strList)
        {
            stream<<str;
        }
        test.close();
    }
}
void Setup_Test::setText10(QString str)
{
    ui->lineEdit_10->setText(str);
    QFile test("/home/pi/test.txt");
    QStringList strList;
    QTextStream stream(&test);
    if (test.exists()&&test.open(QIODevice::ReadOnly))
    {
        while (!test.atEnd())
        {
            strList.append(test.readLine());
        }
        test.close();
    }
    strList[9]=ui->lineEdit_10->text()+"\n";
    if (test.exists()&&test.open(QIODevice::WriteOnly))
    {
        for (QString str:strList)
        {
            stream<<str;
        }
        test.close();
    }
}
void Setup_Test::setText11(QString str)
{
    ui->lineEdit_11->setText(str);
    QFile test("/home/pi/test.txt");
    QStringList strList;
    QTextStream stream(&test);
    if (test.exists()&&test.open(QIODevice::ReadOnly))
    {
        while (!test.atEnd())
        {
            strList.append(test.readLine());
        }
        test.close();
    }
    strList[10]=ui->lineEdit_11->text()+"\n";
    if (test.exists()&&test.open(QIODevice::WriteOnly))
    {
        for (QString str:strList)
        {
            stream<<str;
        }
        test.close();
    }
}
void Setup_Test::setText12(QString str)
{
    ui->lineEdit_12->setText(str);
    QFile test("/home/pi/test.txt");
    QStringList strList;
    QTextStream stream(&test);
    if (test.exists()&&test.open(QIODevice::ReadOnly))
    {
        while (!test.atEnd())
        {
            strList.append(test.readLine());
        }
        test.close();
    }
    strList[11]=ui->lineEdit_12->text()+"\n";
    if (test.exists()&&test.open(QIODevice::WriteOnly))
    {
        for (QString str:strList)
        {
            stream<<str;
        }
        test.close();
    }
}
void Setup_Test::setText13(QString str)
{
    ui->lineEdit_13->setText(str);
    QFile test("/home/pi/test.txt");
    QStringList strList;
    QTextStream stream(&test);
    if (test.exists()&&test.open(QIODevice::ReadOnly))
    {
        while (!test.atEnd())
        {
            strList.append(test.readLine());
        }
        test.close();
    }
    strList[12]=ui->lineEdit_13->text()+"\n";
    if (test.exists()&&test.open(QIODevice::WriteOnly))
    {
        for (QString str:strList)
        {
            stream<<str;
        }
        test.close();
    }
}

void Setup_Test::on_pushButton_2_clicked()
{
    keyboard *key1 = new keyboard(this);
    connect(this,SIGNAL(setOncePoint(int)),key1,SLOT(setOncePoint(int)));
    connect(this,SIGNAL(setKeyString(QString)),key1,SLOT(setKeyString(QString)));
    connect(this,SIGNAL(ClearKey(int)),key1,SLOT(clearKeyK(int)));
    connect(key1,SIGNAL(setText(QString)),this,SLOT(setText1(QString)));

    key1->setModal(true);
    key1->showFullScreen();

    emit ClearKey(2);
    emit setKeyString(ui->lineEdit->text());
    emit setOncePoint(1);
}

void Setup_Test::on_pushButton_3_clicked()
{
    keyboard *key2 = new keyboard(this);
    connect(this,SIGNAL(setOncePoint(int)),key2,SLOT(setOncePoint(int)));
    connect(this,SIGNAL(setKeyString(QString)),key2,SLOT(setKeyString(QString)));
    connect(this,SIGNAL(ClearKey(int)),key2,SLOT(clearKeyK(int)));
    connect(key2,SIGNAL(setText(QString)),this,SLOT(setText2(QString)));

    key2->setModal(true);
    key2->showFullScreen();

    emit ClearKey(2);
    emit setKeyString(ui->lineEdit_2->text());
    emit setOncePoint(1);
}

void Setup_Test::on_pushButton_5_clicked() // клавиатура нижнего порога напряжения смещения
{
    keyboard *key3 = new keyboard(this);
    connect(this,SIGNAL(setOncePoint(int)),key3,SLOT(setOncePoint(int)));
    connect(this,SIGNAL(setKeyString(QString)),key3,SLOT(setKeyString(QString)));
    connect(this,SIGNAL(ClearKey(int)),key3,SLOT(clearKeyK(int)));
    connect(key3,SIGNAL(setText(QString)),this,SLOT(setText3(QString)));

    key3->setModal(true);
    key3->showFullScreen();

    emit ClearKey(5);
    emit setKeyString(ui->lineEdit_3->text());
    emit setOncePoint(1);
    emit setOncePoint(2);

}

void Setup_Test::on_pushButton_4_clicked()
{
    keyboard *key4 = new keyboard(this);
    connect(this,SIGNAL(setOncePoint(int)),key4,SLOT(setOncePoint(int)));
    connect(this,SIGNAL(setKeyString(QString)),key4,SLOT(setKeyString(QString)));
    connect(this,SIGNAL(ClearKey(int)),key4,SLOT(clearKeyK(int)));
    connect(key4,SIGNAL(setText(QString)),this,SLOT(setText4(QString)));

    key4->setModal(true);
    key4->showFullScreen();

    emit ClearKey(2);
    emit setKeyString(ui->lineEdit_4->text());
    emit setOncePoint(1);
}

void Setup_Test::on_pushButton_7_clicked()
{
    keyboard *key5 = new keyboard(this);
    connect(this,SIGNAL(setOncePoint(int)),key5,SLOT(setOncePoint(int)));
    connect(this,SIGNAL(setKeyString(QString)),key5,SLOT(setKeyString(QString)));
    connect(this,SIGNAL(ClearKey(int)),key5,SLOT(clearKeyK(int)));
    connect(key5,SIGNAL(setText(QString)),this,SLOT(setText5(QString)));

    key5->setModal(true);
    key5->showFullScreen();

    emit ClearKey(2);
    emit setKeyString(ui->lineEdit_5->text());
    emit setOncePoint(1);
}

void Setup_Test::on_pushButton_6_clicked()
{
    keyboard *key6 = new keyboard(this);
    connect(this,SIGNAL(setOncePoint(int)),key6,SLOT(setOncePoint(int)));
    connect(this,SIGNAL(setKeyString(QString)),key6,SLOT(setKeyString(QString)));
    connect(this,SIGNAL(ClearKey(int)),key6,SLOT(clearKeyK(int)));
    connect(key6,SIGNAL(setText(QString)),this,SLOT(setText6(QString)));

    key6->setModal(true);
    key6->showFullScreen();

    emit ClearKey(2);
    emit setKeyString(ui->lineEdit_6->text());
    emit setOncePoint(1);
}

void Setup_Test::on_pushButton_9_clicked()
{
    keyboard *key7 = new keyboard(this);
    connect(this,SIGNAL(setOncePoint(int)),key7,SLOT(setOncePoint(int)));
    connect(this,SIGNAL(setKeyString(QString)),key7,SLOT(setKeyString(QString)));
    connect(this,SIGNAL(ClearKey(int)),key7,SLOT(clearKeyK(int)));
    connect(key7,SIGNAL(setText(QString)),this,SLOT(setText7(QString)));

    key7->setModal(true);
    key7->showFullScreen();

    emit ClearKey(2);
    emit setKeyString(ui->lineEdit_7->text());
    emit setOncePoint(1);
}

void Setup_Test::on_pushButton_8_clicked()
{
    keyboard *key8 = new keyboard(this);
    connect(this,SIGNAL(setOncePoint(int)),key8,SLOT(setOncePoint(int)));
    connect(this,SIGNAL(setKeyString(QString)),key8,SLOT(setKeyString(QString)));
    connect(this,SIGNAL(ClearKey(int)),key8,SLOT(clearKeyK(int)));
    connect(key8,SIGNAL(setText(QString)),this,SLOT(setText8(QString)));

    key8->setModal(true);
    key8->showFullScreen();

    emit ClearKey(2);
    emit setKeyString(ui->lineEdit_8->text());
    emit setOncePoint(1);
}

void Setup_Test::on_pushButton_11_clicked()
{
    keyboard *key9 = new keyboard(this);
    connect(this,SIGNAL(setOncePoint(int)),key9,SLOT(setOncePoint(int)));
    connect(this,SIGNAL(setKeyString(QString)),key9,SLOT(setKeyString(QString)));
    connect(this,SIGNAL(ClearKey(int)),key9,SLOT(clearKeyK(int)));
    connect(key9,SIGNAL(setText(QString)),this,SLOT(setText9(QString)));

    key9->setModal(true);
    key9->showFullScreen();

    emit ClearKey(2);
    emit setKeyString(ui->lineEdit_9->text());
    emit setOncePoint(1);
}

void Setup_Test::on_pushButton_10_clicked()
{
    keyboard *key10 = new keyboard(this);
    connect(this,SIGNAL(setOncePoint(int)),key10,SLOT(setOncePoint(int)));
    connect(this,SIGNAL(setKeyString(QString)),key10,SLOT(setKeyString(QString)));
    connect(this,SIGNAL(ClearKey(int)),key10,SLOT(clearKeyK(int)));
    connect(key10,SIGNAL(setText(QString)),this,SLOT(setText10(QString)));

    key10->setModal(true);
    key10->showFullScreen();

    emit ClearKey(2);
    emit setKeyString(ui->lineEdit_10->text());
    emit setOncePoint(1);
}

void Setup_Test::on_pushButton_13_clicked() // кнопка клавиатуры нижнего порога тока
{
    keyboard *key11 = new keyboard(this);
    connect(this,SIGNAL(setOncePoint(int)),key11,SLOT(setOncePoint(int)));
    connect(this,SIGNAL(setKeyString(QString)),key11,SLOT(setKeyString(QString)));
    connect(this,SIGNAL(ClearKey(int)),key11,SLOT(clearKeyK(int)));
    connect(key11,SIGNAL(setText(QString)),this,SLOT(setText11(QString)));

    key11->setModal(true);
    key11->showFullScreen();

    emit ClearKey(5);
    emit setKeyString(ui->lineEdit_11->text());
    emit setOncePoint(1);
    emit setOncePoint(2);
}

void Setup_Test::on_pushButton_12_clicked()
{
    keyboard *key12 = new keyboard(this);
    connect(this,SIGNAL(setOncePoint(int)),key12,SLOT(setOncePoint(int)));
    connect(this,SIGNAL(setKeyString(QString)),key12,SLOT(setKeyString(QString)));
    connect(this,SIGNAL(ClearKey(int)),key12,SLOT(clearKeyK(int)));
    connect(key12,SIGNAL(setText(QString)),this,SLOT(setText12(QString)));

    key12->setModal(true);
    key12->showFullScreen();

    emit ClearKey(2);
    emit setKeyString(ui->lineEdit_12->text());
    emit setOncePoint(1);

}

void Setup_Test::on_pushButton_14_clicked()
{
    keyboard *key13 = new keyboard(this);
    connect(this,SIGNAL(setOncePoint(int)),key13,SLOT(setOncePoint(int)));
    connect(this,SIGNAL(setKeyString(QString)),key13,SLOT(setKeyString(QString)));
    connect(this,SIGNAL(ClearKey(int)),key13,SLOT(clearKeyK(int)));
    connect(key13,SIGNAL(setText(QString)),this,SLOT(setText13(QString)));

    key13->setModal(true);
    key13->showFullScreen();

    emit ClearKey(3);
    emit setKeyString(ui->lineEdit_13->text());
    emit setOncePoint(2);
}
