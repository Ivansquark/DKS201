// Окно Настройки***************************
#ifndef SETUP_H
#define SETUP_H

#include <QDialog>
#include "setup_commonsetup.h"
#include "setup_equipment.h"
#include "setup_test.h"
#include <QFile>

namespace Ui {
class Setup;
}

class Setup : public QDialog
{
    Q_OBJECT

public:
    explicit Setup(QWidget *parent = nullptr);
    ~Setup();


private slots:
    void on_pushButton_4_clicked();
    void on_pushButton_clicked();
    void constr(int x);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Setup *ui;
    Setup_commonSetup *commSet;
    Setup_equipment *equipSet;
    Setup_Test *testSet;
protected:
    //void closeEvent(QCloseEvent *event);
};

#endif // SETUP_H
