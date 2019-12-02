#ifndef ADD_CHAMBER_H
#define ADD_CHAMBER_H

#include <QDialog>
#include <QSql>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlDriver>
#include <QSqlRecord>
#include "keyboard.h"

namespace Ui {
class Add_chamber;
}

class Add_chamber : public QDialog
{
    Q_OBJECT

public:
    explicit Add_chamber(QWidget *parent = nullptr);
    ~Add_chamber();

signals:
    void newCombo();
    void clearKeyK(int);
    void setKeyString(QString str);
    void setOncePoint(int);

public slots:
    void constr(QString num);
    void setTextLine_1(QString);
    void setTextLine_2(QString str);
    void setTextLine_3(QString str);
    void setTextLine_4(QString str);
    void setTextLine_5(QString str);
    void setTextLine_6(QString str);
    void setTextLine_7(QString str);


private slots:
    void on_pushButton_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

private:
    Ui::Add_chamber *ui;
};

#endif // ADD_CHAMBER_H
