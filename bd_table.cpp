#include "bd_table.h"
#include "ui_bd_table.h"

BD_table::BD_table(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BD_table)
{
    ui->setupUi(this);
}

BD_table::~BD_table()
{
    delete ui;
}

void BD_table::init(QString tableName)
{
    TableName=tableName;
    QString axisY="";
    QString axisY1=tableName.right(1);
    //qDebug()<<tableName<<axisY1;
    if (axisY1=="А")
    {axisY=axisY1;}
    QString axisY2=tableName.right(2);
    if (axisY2=="Кл") {axisY=axisY2;}
    if (axisY2=="Гр") {axisY=axisY2;}
    if (axisY2=="Зв") {axisY=axisY2;}
    QString axisY3=tableName.right(3);
    if (axisY3=="Р_с") {axisY=axisY3;}
    if (axisY3=="Р_ч") {axisY=axisY3;}
    QString axisY4=tableName.right(4);
    if (axisY4=="Зв_с") {axisY=axisY4;}
    if (axisY4=="Зв_ч") {axisY=axisY4;}
    if (axisY4=="Гр_с") {axisY=axisY4;}
    if (axisY4=="Гр_ч") {axisY=axisY4;}
    QString axisY5=tableName.right(5);
    if (axisY5=="Р_мин") {axisY=axisY5;}
    QString axisY6=tableName.right(6);
    if (axisY6=="Зв_мин") {axisY=axisY6;}
    if (axisY6=="Гр_мин") {axisY=axisY6;}
    QSqlDatabase::addDatabase("QSQLITE","myDB1");
    QStringList lst_Second,lst_Value;
    if(QSqlDatabase::contains("myDB1"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB1");
        db.setDatabaseName("/home/pi/mes.db");
        if(!db.open()){qDebug()<<"db was not opened";}
        QSqlQuery query(db);
        QString strQuery1 = "SELECT *FROM %1";
        QString strQuery=strQuery1.arg(TableName);
        if(!query.exec(strQuery)){qDebug()<<"unable execute query SELECT";}
        while(query.next())
        {
            lst_Second.push_back(query.value("second").toString());
            lst_Value.push_back(query.value("value").toString());
        }
    }

    QVector<double>x,y; // Объявляем вектор значений

    foreach(auto &i,lst_Second)
    {
        x.push_back(i.toDouble());
    }
    foreach(auto &i,lst_Value)
    {
        y.push_back(i.toDouble());
    }
    //x.push_front(0);
    //y.push_front(0);
    //qDebug()<<x<<y;
    QFont serifFont("Times", 10, QFont::Bold);
    QFont serifFont1("Times", 12, QFont::Bold);
    customPlot = new QCustomPlot(); // Инициализируем графическое полотно
    okBut=new QPushButton(this);
    okBut->setText("Ок");
    okBut->setMinimumSize(100,50);
    okBut->setMaximumSize(200,100);
    cancelBut=new QPushButton(this);
    cancelBut->setText("Назад");
    cancelBut->setMinimumSize(50,50);    
    connect(cancelBut,SIGNAL(clicked()),this,SLOT(onCancelClick()));
    lineX=new QLineEdit(this);
    lineX->setText("сек");
    lineX->setFont(serifFont1);
    lineX->setMaximumWidth(200);
    lineY=new QLineEdit(this);
    lineY->setText(axisY);
    lineY->setFont(serifFont1);
    lineY->setMaximumWidth(200);
    labelX=new QLabel(this);
    labelX->setText("Ось X");
    labelY=new QLabel(this);
    labelY->setText("Ось Y");
    //view = new QGraphicsView(this);
    table = new QTableWidget(this);
    table->setRowCount(lst_Second.length());
    table->setColumnCount(2);
    table->setColumnWidth(0,50);
    table->setColumnWidth(1,200);
    table->setBackgroundRole(QPalette::Dark);
    table->setFixedWidth(200);
    table->setFont(serifFont);
    for(int i=0; i<lst_Second.size();i++)
    {
        QTableWidgetItem *item=new QTableWidgetItem();
        QTableWidgetItem *item1=new QTableWidgetItem();
        item->setText(lst_Second[i]);//вставлен элемент списка
        item1->setText(lst_Value[i]);//вставлен элемент списка
        table->setItem(i,0,item);
        table->item(i,0)->setBackground(Qt::black);
        table->setItem(i,1,item1);
        table->item(i,1)->setBackground(Qt::gray);
    }
    table->verticalHeader()->hide();
    table->horizontalHeader()->setStyleSheet("QHeaderView::section {color:black, background-color:red, border:10px solid black;}");
    table->horizontalHeader()->setVisible(false);
    grid=new QGridLayout(this);
    grid->addWidget(labelX,0,0);
    grid->addWidget(lineX,1,0);
    grid->addWidget(labelY,2,0);
    grid->addWidget(lineY,3,0);
    grid->addWidget(table,4,0);
    grid->addWidget(okBut,5,0);
    grid->addWidget(cancelBut,5,1);
    grid->addWidget(customPlot,0,1,5,1);  // Устанавливаем customPlot в окно приложения
    setLayout(grid);

    customPlot->setMinimumSize(600,300);
    customPlot->setInteraction(QCP::iRangeZoom,true);   // Включаем взаимодействие удаления/приближения
    customPlot->setInteraction(QCP::iRangeDrag, true);  // Включаем взаимодействие перетаскивания графика
    //customPlot->axisRect()->setRangeDrag(Qt::Horizontal);   // Включаем перетаскивание только по горизонтальной оси
    //customPlot->axisRect()->setRangeZoom(Qt::Horizontal);   // Включаем удаление/приближение только по горизонтальной оси
    //customPlot->axisRect()->setRangeDrag(Qt::Vertical);   // Включаем перетаскивание только по горизонтальной оси
    //customPlot->axisRect()->setRangeZoom(Qt::Vertical);   // Включаем удаление/приближение только по горизонтальной оси
    customPlot->xAxis->setLabel("сек"); //setTickLabelType(QCPAxis::ltDateTime);   // Подпись координат по Оси X в качестве Даты и Времени
    customPlot->yAxis->setLabel(axisY);
    //customPlot->xAxis->setDateTimeFormat("mm");  // Устанавливаем формат даты и времени
    customPlot->setBackground(Qt::white);

    // Настраиваем шрифт по осям координат
    customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 14));
    customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 14));

    // Автоматическое масштабирование тиков по Оси X и Y
    customPlot->xAxis->setAutoTickStep(true);
    customPlot->yAxis->setAutoTickStep(true);

    /* Делаем видимыми оси X и Y по верхней и правой границам графика,
    * но отключаем на них тики и подписи координат
    * */
    customPlot->xAxis2->setVisible(true);
    customPlot->yAxis2->setVisible(true);
    //customPlot->xAxis2->setTicks(false);
    //customPlot->yAxis2->setTicks(false);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->yAxis2->setTickLabels(false);

    customPlot->yAxis->setTickLabelColor(QColor(Qt::blue)); // Голубой цвет подписей тиков по Оси Y
    customPlot->xAxis->setTickLabelColor(QColor(Qt::green)); //  цвет подписей тиков по Оси X
    //customPlot->legend->setVisible(true);   //Включаем Легенду графика
    // Устанавливаем Легенду в левый верхний угол графика
    //customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);

    // Инициализируем график и привязываем его к Осям
    graphic = new QCPGraph(customPlot->xAxis, customPlot->yAxis);
    customPlot->addPlottable(graphic);  // Устанавливаем график на полотно
    graphic->setName("Доза");       // Устанавливаем
    graphic->setPen(QPen(Qt::red,2,Qt::SolidLine)); // Устанавливаем цвет графика и толщину
    //graphic->setBrush(QBrush(QColor(Qt::red))); //заполнение под графиком
    graphic->setAntialiased(false);         // Отключаем сглаживание, по умолчанию включено
    graphic->setLineStyle(QCPGraph::lsLine); // График в виде импульсных тиков //линии

    /* Подключаем сигнал от Оси X об изменении видимого диапазона координат
    * к СЛОТу для переустановки формата времени оси.
    * */
    //connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)),this, SLOT(slotRangeChanged(QCPRange)));

    graphic->setData(x, y); // Устанавливаем данные
    customPlot->rescaleAxes();      // Масштабируем график по данным
    customPlot->replot();           // Отрисовываем график
}
