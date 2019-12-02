// Главное окно
#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), ui(new Ui::MainWindow) // конструктор
{
    C=0;
    ui->setupUi(this);
    connect(this,SIGNAL(sendTestBegin()),this,SLOT(recieveTestBegin())); // -создаем этот коннект в конструкторе так как при вызове с кнопки вызывается несколько раз

    setup = new Setup(); // указатель на (новое) окно настроек
    key = new class keyboard(this);  // указательна окно клавиатуры

    connect(key,SIGNAL(setText(QString)),this,SLOT(setText1(QString)));    
    connect(this,SIGNAL(clearKeyM(int)),key,SLOT(clearKeyK(int)));
    connect(this,SIGNAL(constrSET(int)),setup,SLOT(constr(int)));


    QFile password("/home/pi/password1.txt");
    QString str="",str2="",str3="";
    QStringList strList;
    if ((password.exists())&&(password.open(QIODevice::ReadOnly)))
    {
        while(!password.atEnd())
        {
            strList.append(password.readLine());
        }
        password.close();
    }
    strList[0]="1\n";
    if ((password.exists())&&password.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&password);
        //QTextCodec *codec1251=QTextCodec::codecForName("Windows-1251");
        //QTextCodec *codecUTF8=QTextCodec::codecForName("UTF-8");

        foreach(QString s,strList)
        {
            //QByteArray array;
            //array.append(s);
            //QString resultUnicode = codec1251->toUnicode(array);
            //QString strUTF8=codecUTF8->fromUnicode(resultUnicode);

            //stream<<strUTF8;
            stream<<s;
        }
        password.close();

    }

    if (strList[0]==strList[1])
    {
        ui->label_pass->setText("ПОВЕРИТЕЛЬ");
    }
    else
    {
        if (strList[0]==strList[2])
        {
           ui->label_pass->setText("ИЗГОТОВИТЕЛЬ");
        }
        else
        {
           ui->label_pass->setText("ОПЕРАТОР");
        }
    }
    ui->Exit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);    
}

MainWindow::~MainWindow() // деструктор
{
    delete ui;
    delete setup;
    delete key;
}


void MainWindow::setText1(QString str)  // функция принимает сигнал от окна ввода пароля, устанавливает первую строку в файле пароль, и;
{
    if(str!="")
    {
        QFile password("/home/pi/password1.txt");
        QStringList strList;
        if ((password.exists())&&password.open(QIODevice::ReadOnly))
        {
            while(!password.atEnd())
            {
                strList.append(password.readLine());
            }
            password.close();
        }
        if ((password.exists())&&password.open(QIODevice::WriteOnly))
        {
            strList[0]=str+"\n";
            QTextStream stream(&password);
            foreach(QString s,strList)
            {
                //QByteArray array;
                //array.append(s);
                //QTextCodec *Codec1251=QTextCodec::codecForName("Windows-1251");
                //QString ResultUnicode=Codec1251->toUnicode(array);
                //QTextCodec *CodecUTF8=QTextCodec::codecForName("UTF-8");
                //QString strUTF8=CodecUTF8->fromUnicode(ResultUnicode);
                //stream<<strUTF8;
                stream<<s;
            }
            password.close();
        }

        if (strList[0]==strList[1])
        {
            ui->label_pass->setText("ПОВЕРИТЕЛЬ");
        }
        else
        {
            if (strList[0]==strList[2])
            {
                ui->label_pass->setText("ИЗГОТОВИТЕЛЬ");
            }
            else
            {
                ui->label_pass->setText("ОПЕРАТОР");
            }
        }
    }
emit clearKeyM(1); // - сигнал очищающий строку в окне клавиатуры;
}


void MainWindow::on_Exit_clicked() // Exit
{
    close();
}

void MainWindow::on_NewWin_clicked() // кнопка вызова окна настроек
{
    setup->setModal(true);
    setup->showFullScreen();
    if (ui->label_pass->text()=="ОПЕРАТОР")
    {
        emit constrSET(1); //сигнал по состоянию ОПЕРАТОР
    }
    else emit constrSET(2); // сигнал в Setup по состоянию ПОВЕРИТЕЛЬ,ИЗГОТОВИТЕЛЬ
}


void MainWindow::on_pushButton_clicked() // кнопка вызова окна с клавиатурой для ввода кода доступа;
{

    key->setModal(true);
    key->showFullScreen();
    emit clearKeyM(1); //сигнал управления клавиатурой;
}



void MainWindow::on_pushButton_3_clicked() // кнопка измерение (проверяется включен ли эмулятор или СОМ порт)
{
    QString str;
    QFile config("/home/pi/config.txt");
    if(config.exists()&&config.open(QIODevice::ReadOnly))
    {
        str.append(config.readLine());
        config.close();
    }
    if (str=="2\n") // проверка измерения на эмулятор
    {
        EmulHex *emul=new EmulHex;
        emul->setModal(false);
        connect(this,SIGNAL(sendEMUL()),emul,SLOT(recieveEMUL()));
        connect(emul,SIGNAL(sendTestOff4()),this,SLOT(recieveTestOff()));
        emit sendEMUL();
    }
    if (str=="1\n") // проверка измерения на СОМ
    {        
        Com *PortNew = new Com(this);
        QThread *thread_New = new QThread();

        connect(thread_New, SIGNAL(started()), PortNew, SLOT(process_Port()));//Переназначения метода run !!!!!!!!
        connect(PortNew,SIGNAL(finished_Port()),thread_New,SLOT(quit())); //Переназначение метода выход
        connect(thread_New,SIGNAL(finished()),thread_New,SLOT(deleteLater()));
        connect(thread_New,SIGNAL(finished()),PortNew,SLOT(deleteLater()));

        PortNew->moveToThread(thread_New);//помешаем класс  в поток
        PortNew->port->moveToThread(thread_New);//Помещаем сам порт в поток

        connect(this, SIGNAL(ConnectPort()),PortNew,SLOT(ConnectPort()));//по нажатию кнопки подключить порт
        connect(this, SIGNAL(DisconnectPort()),PortNew,SLOT(DisconnectPort()));//по нажатию кнопки отключить порт
        connect(this,SIGNAL(writeData(QByteArray)),PortNew,SLOT(WriteToPort(QByteArray)));//отправка в порт данных
        //connect(this,SIGNAL(closePort()),PortNew,SLOT(closePort()));//закрыть класс порта

        connect(PortNew, SIGNAL(outPort(QString)), this, SLOT(print(QString)));//вывод в текстовое поле считанных данных и передача их дальше        
        connect(PortNew,SIGNAL(process_Port_Start()),this,SLOT(process_Port_Start()));

        thread_New->start();

        AverADC_COM *avrADC=new AverADC_COM;
        //connect(this,SIGNAL(sendTestBegin()),avrADC,SLOT(recieveTestBegin()));

        connect(this,SIGNAL(COMsend(QString)),avrADC,SLOT(recieveCOM(QString)));
        connect(this,SIGNAL(close2()),avrADC,SLOT(close2()));

        connect(avrADC,SIGNAL(sendADC(QString)),this,SLOT(recieveADC(QString)));
        //connect(avrADC,SIGNAL(close1()),this,SLOT(close1()));
        //connect(avrADC,SIGNAL(sendTestOff2()),this,SLOT(sendTestOff3()));
        emit sendTestBegin();
        //emit ConnectPort();
    }
}

void MainWindow::recieveTestBegin() // Отображение окна Теста по СОМ порту
{
    C=C+1;
    qDebug()<<C;
    TestCom *test = new TestCom(this);
    connect(this,SIGNAL(sendADCdec(QString)),test,SLOT(recieveADCdec(QString)));
    connect(test,SIGNAL(sendToCom(QByteArray)),this,SLOT(sendToComFromTest(QByteArray)));
    connect(test,SIGNAL(testEnd()),this,SLOT(testEnd()));
    connect(test,SIGNAL(closePortTest()),this,SLOT(closePortTest()));
    test->setModal(true);
    test->showFullScreen();
}
void MainWindow::closePortTest() //функция приема сигнала от окна теста закрывающая порт (чтобы можно было заново запустить измерение)
{
    emit DisconnectPort();
    emit close2();
    //emit closePort();
}
void MainWindow::testEnd()             // функция окончания теста от СОМ порта и вызова окна результата теста;
{
    Measuring_Popup *testResult = new Measuring_Popup(this);
    connect(testResult,SIGNAL(repeatTest()),this,SLOT(repeatTest()));
    connect(testResult,SIGNAL(okTest()),this,SLOT(okTest()));
    testResult->setModal(true);
    testResult->showFullScreen();
}
void MainWindow::repeatTest() // функция приема сигнала от кнопки повторить тест
{
    ui->pushButton_3->click();
}
void MainWindow::okTest() // функция приема сигнала от кнопки ok теста
{
    Chambers *cham = new Chambers();
    cham->setModal(true);
    cham->showFullScreen();
    connect(this,SIGNAL(firstTimeFlag()),cham,SLOT(firstTimeFlag()));
    connect(cham,SIGNAL(okChambers()),this,SLOT(okChambers()));
    emit firstTimeFlag();
}

void MainWindow::okChambers() //функция приема сигнала от кнопки ok камер впервые
{
    Measurement_config *mesConf= new Measurement_config(this);
    mesConf->setModal(true);
    mesConf->showFullScreen();
    connect(this,SIGNAL(firstTimeFlag()),mesConf,SLOT(firstTimeFlag()));
    connect(mesConf,SIGNAL(okMesConf()),this,SLOT(okMesConf()));
    connect(mesConf,SIGNAL(bytesVolt(QByteArray)),this,SLOT(bytesVolt(QByteArray)));
    emit firstTimeFlag();
}
void MainWindow::bytesVolt(QByteArray data) //слот принятия данных о текущем высоком напряжении
{
    qDebug()<<QString::number(data[0],16)<<QString::number(data[1],16);
    //emit writeData(data);
}
void MainWindow::okMesConf() // сигнал о нажатии ок из окна измерения (во второй раз)
{
    Measuring *mes=new Measuring(this);
    connect(this,SIGNAL(sendADCdec(QString)),mes,SLOT(recieveADC_COM(QString)));
    connect(mes,SIGNAL(closePortTest()),this,SLOT(closePortTest()));
    connect(mes,SIGNAL(bytesVoltM(QByteArray)),SLOT(bytesVoltM(QByteArray))); // коннект сигнала с байтами высокого напряжения из окна измерения (во второй раз)
    connect(mes,SIGNAL(sendToPortMainW(QByteArray)),this,SLOT(sendToPortMainW(QByteArray)));
    mes->setModal(true);
    mes->showFullScreen();
    //emit mesCOM();
}
void MainWindow::sendToPortMainW(QByteArray data) // посылаем байты в порт команду включения или выключения доп фильтра (С1 - 560 пФ)
{
    emit writeData(data);
}
void MainWindow::bytesVoltM(QByteArray data)// посылаем байты в порт команды высокого напряжения
{
    qDebug()<<QString::number(data[0],16)<<QString::number(data[1],16);
    //emit writeData(data);
}


void MainWindow::sendToComFromTest(QByteArray data) // посылаем байты в порт
{
    emit writeData(data);
    //PortNew->WriteToPort(data);
    //qDebug()<<QString::number(data[0],16)<<QString::number(data[1],16)<<QString::number(data[2],16);
}
void MainWindow::process_Port_Start() // функция подключения порта
{
    emit ConnectPort();
}
void MainWindow::recieveADC(QString str) // посылка значений АЦП в Тест и в окно измерений по окончанию теста;
{
    emit sendADCdec(str);
}
void MainWindow::DisconnectPortRecieve() // функция отключения порта
{
    emit DisconnectPort();
}
void MainWindow::print(QString COM)//чтение с СОМ порта строки и пересылка в AverADC_COM
{
    emit COMsend(COM);   
}
void MainWindow::writeToComRecieve(QByteArray data) // посылаем байты в порт
{
    emit writeData(data);
}



void MainWindow::recieveTestOff() //функция запуска окна результатов Теста от Эмулятора
{
    Measuring_Popup *testResult = new Measuring_Popup(this);
    testResult->setModal(true);
    testResult->showFullScreen();
}

void MainWindow::on_pushButton_2_clicked() // временная кнопка выбора камеры
{
    Chambers* chamber = new Chambers(this);
    chamber->setModal(true);
    chamber->showFullScreen();
}

void MainWindow::on_pushButton_4_clicked() //временная кнопка настроек
{
    Measurement_config *measure = new Measurement_config(this);
    measure->setModal(true);
    measure->showFullScreen();
}

void MainWindow::on_pushButton_5_clicked() // временная кнопка измерений
{
    Measuring *mes = new Measuring(this);    
    mes->setModal(true);
    mes->showFullScreen();
}

void MainWindow::on_pushButton_6_clicked() // временная кнопка параметров
{
    Measuring_parameters *par = new Measuring_parameters(this);
    par->setModal(true);
    par->showFullScreen();
}

void MainWindow::on_pushButton_7_clicked() // временная кнопка измерений с эмулятором
{
    EmulHex *emul1=new EmulHex();
    emul1->setModal(false);
    connect(this,SIGNAL(sendMesBegin()),emul1,SLOT(recieveMesBegin()));
    //connect(emul1,SIGNAL(del()),this,SLOT(delEmul()));
    emit sendMesBegin();
    //delete emul1;
}





