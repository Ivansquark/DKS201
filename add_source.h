#ifndef ADD_SOURCE_H
#define ADD_SOURCE_H

#include <QDialog>
#include <QFile>
#include <QSql>
#include <QSqlQuery>
#include <QDebug>
#include <QDate>

#include "keyboard.h"
#include "date.h"
#include "math.h"

namespace Ui {
class Add_source;
}

class Add_source : public QDialog
{
    Q_OBJECT

public:
    explicit Add_source(QWidget *parent = nullptr);
    ~Add_source();

    void init1();
    void init2();

signals:
     void setKeyString(QString);
     void clearKeyK(int);
     void newSourceList();

public slots:
     void setTextLine_1(QString);
     void setTextLine_2(QString str);
     void setTextLine_3(QString str);
     void setTextLine_4(QString str);
     void setTextLine_5(QString str);
     void setTextLine_6(QString str);
     //void setTextLine_7(QString str);
     void recieveDate1(int,int,int);
     void recieveDate2(int,int,int);
     void constr(QString);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_9_clicked();

private:
    Ui::Add_source *ui;
    QDate dateFabrication, dateVerification, dateCurrent;
    double doseRateCurrent, doseRateVerification;
    double activityCurrent, activityFabrication;
    double half_life;
};

#endif // ADD_SOURCE_H
