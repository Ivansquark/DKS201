#ifndef VOLTAGE_H
#define VOLTAGE_H

#include <QDialog>
#include <QSql>
#include <QSqlQuery>
#include "keyboard.h"

namespace Ui {
class Voltage;
}

class Voltage : public QDialog
{
    Q_OBJECT

public:
    explicit Voltage(QWidget *parent = nullptr);
    ~Voltage();

signals:
    void clearKeyK(int);
    void setOncePoint(int);
    void sendString(QString);
    void sendVolt(QString);

public slots:
    void recieveVolt(QString);
    void setText(QString);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_radioButton_5_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_4_clicked();

private:
    Ui::Voltage *ui;
    QString voltageStr;
    int voltageInt;
};

#endif // VOLTAGE_H
