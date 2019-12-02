// Окно Настройки-общие настройки***************************

#ifndef SETUP_COMMONSETUP_H
#define SETUP_COMMONSETUP_H

#include <QDialog>
#include "setup_commonsetup_userlist.h"
#include <QFile>
#include <QTextStream>

namespace Ui {
class Setup_commonSetup;
}

class Setup_commonSetup : public QDialog
{
    Q_OBJECT

public:
    explicit Setup_commonSetup(QWidget *parent = nullptr);
    ~Setup_commonSetup();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_4_clicked();

private:
    Ui::Setup_commonSetup *ui;
    Setup_CommonSetup_UserList *userlist;
protected:
    //void closeEvent(QCloseEvent *event);
};

#endif // SETUP_COMMONSETUP_H
