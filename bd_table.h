#ifndef BD_TABLE_H
#define BD_TABLE_H

#include <QDialog>
#include <QSql>
#include <QSqlQuery>
#include <QDebug>
#include <QGridLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QLineEdit>
#include <QGraphicsView>

#include "qcustomplot.h"

namespace Ui {
class BD_table;
}

class BD_table : public QDialog
{
    Q_OBJECT

public:
    explicit BD_table(QWidget *parent = nullptr);
    ~BD_table();
public:
    void init(QString);
private slots:
    void onCancelClick();
    void onOkClick();

private:
    Ui::BD_table *ui;
    QPushButton *okBut,*cancelBut;
    QLabel *labelX,*labelY;
    QLineEdit *lineX,*lineY;
    QGraphicsView *view;
    QGridLayout *grid;
    QTableWidget *table;

    QString TableName;


    QCustomPlot *customPlot;    // Объявляем графическое полотно
    QCPGraph *graphic;          // Объявляем график

};

#endif // BD_TABLE_H
