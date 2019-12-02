#ifndef SETUP_EQUIPMENT_H
#define SETUP_EQUIPMENT_H

#include <QDialog>
#include "setup_equipment_advanced_settings.h"
#include "keyboard.h"
#include <QFile>
#include <QTextStream>
#include <QDoubleValidator>

namespace Ui {
class Setup_equipment;
}

class Setup_equipment : public QDialog
{
    Q_OBJECT

public:
    explicit Setup_equipment(QWidget *parent = nullptr);
    ~Setup_equipment();
signals:
    void setKey1(int x);
    void setKeyString(QString str);
    void setOncePoint(int x);

public slots:
    void setText1(QString str);
    void setText2(QString str);
    void setText3(QString str);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_5_clicked();
    void on_radioButton_clicked();
    void on_radioButton_2_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

private:
    Ui::Setup_equipment *ui;
    Setup_Equipment_Advanced_Settings *setEqAS;
    keyboard *key1;
    keyboard *key2;
    keyboard *key3;
    QDoubleValidator Validator;
protected slots:
    void on_changed();

};

#endif // SETUP_EQUIPMENT_H
