// Окно Настройки-общие настройки-список пользователей-поверитель***************************

#ifndef SETUP_COMMONSETUP_USERLIST_SETOPERATOR_H
#define SETUP_COMMONSETUP_USERLIST_SETOPERATOR_H

#include <QDialog>
#include <QMessageBox>
#include "keyboard.h"
#include <QFile>
#include <QTextStream>
#include <QTextCodec>


namespace Ui {
class Setup_CommonSetup_UserList_SetOperator;
}

class Setup_CommonSetup_UserList_SetOperator : public QDialog
{
    Q_OBJECT

public:
    explicit Setup_CommonSetup_UserList_SetOperator(QWidget *parent = nullptr);
    ~Setup_CommonSetup_UserList_SetOperator();

public slots:
    void setLine1(QString str);
    void setLine2(QString str);
private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::Setup_CommonSetup_UserList_SetOperator *ui;
    keyboard *key1;
    keyboard *key2;
};

#endif // SETUP_COMMONSETUP_USERLIST_SETOPERATOR_H
