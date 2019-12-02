#ifndef MEASURING_MODE_H
#define MEASURING_MODE_H

#include <QDialog>
#include <QSql>
#include <QSqlQuery>
#include <QDebug>

#include "keyboard.h"


namespace Ui {
class Measuring_mode;
}

class Measuring_mode : public QDialog
{
    Q_OBJECT

public:
    explicit Measuring_mode(QWidget *parent = nullptr);
    ~Measuring_mode();
signals:
    void sendStr(QString);
    void clearKeyK(int);
    void sendString(QString);

public slots:
    void setText(QString);

private slots:
    void on_pushButton_2_clicked();

    void on_radioButton_4_clicked();

    void on_radioButton_5_clicked();

    void on_pushButton_clicked();

    void on_pushButton_8_clicked();

private:
    Ui::Measuring_mode *ui;
};

#endif // MEASURING_MODE_H
