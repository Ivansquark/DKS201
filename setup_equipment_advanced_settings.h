#ifndef SETUP_EQUIPMENT_ADVANCED_SETTINGS_H
#define SETUP_EQUIPMENT_ADVANCED_SETTINGS_H

#include <QDialog>
#include <QFile>
#include <QTextStream>
#include "keyboard.h"

namespace Ui {
class Setup_Equipment_Advanced_Settings;
}

class Setup_Equipment_Advanced_Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Setup_Equipment_Advanced_Settings(QWidget *parent = nullptr);
    ~Setup_Equipment_Advanced_Settings();
signals:
    void setKeyString(QString str);
    void setOncePoint(int x);
    void ClearKey(int x);

public slots:
    void setText1(QString str);
    void setText2(QString str);
    void setText3(QString str);
    void setText4(QString str);
    void setText5(QString str);
    void setText6(QString str);
    void setText7(QString str);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_8_clicked();

private:
    Ui::Setup_Equipment_Advanced_Settings *ui;    
};

#endif // SETUP_EQUIPMENT_ADVANCED_SETTINGS_H
