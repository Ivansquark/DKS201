#ifndef MEASUREMENT_CONFIG_H
#define MEASUREMENT_CONFIG_H

#include <QDialog>
#include <QDebug>
#include <QSql>
#include <QSqlQuery>
#include <QMessageBox>

#include "chambers.h"
#include "voltage.h"
#include "volthex.h"

namespace Ui {
class Measurement_config;
}

class Measurement_config : public QDialog
{
    Q_OBJECT

public:
    explicit Measurement_config(QWidget *parent = nullptr);
    ~Measurement_config();

signals:
    void sendString(QString);
    void setOncePoint(int);
    void clearKeyK(int);
    void sendVolt(QString);
    void updateMes();
    void okMesConf(); // сигнал о нажатии ок впервые для взова следующего окна
    void bytesVolt(QByteArray); // сигнал с байтами высокого напряжения из окна после теста
    void bytesVolt1(QByteArray); // сигнал с байтами высокого напряжения из окна в процессе измерения
public slots:
    void setTextTemp(QString);
    void setTextPress(QString str);
    void setTextCor_fact(QString str);
    void setTextThreashold_dose(QString);
    void setTextThreashold_time(QString str);
    void updateData();
    void firstTimeFlag(); // флаг о вызове окна в первый раз

    void recieveVolt(QString);


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::Measurement_config *ui;
    int F;
};

#endif // MEASUREMENT_CONFIG_H
