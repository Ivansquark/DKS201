// Окно Настройки-общие настройки-список пользователей-изготовитель***************************

#ifndef SETUP_COMMONSETUP_USERLIST_SETMANUFACTORER_H
#define SETUP_COMMONSETUP_USERLIST_SETMANUFACTORER_H

#include <QDialog>
#include "keyboard.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
namespace Ui {
class Setup_CommonSetup_UserList_SetManufactorer;
}

class Setup_CommonSetup_UserList_SetManufactorer : public QDialog
{
    Q_OBJECT

public:
    explicit Setup_CommonSetup_UserList_SetManufactorer(QWidget *parent = nullptr);
    ~Setup_CommonSetup_UserList_SetManufactorer();
public slots:
    void setLine1(QString str);
    void setLine2(QString str);


private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::Setup_CommonSetup_UserList_SetManufactorer *ui;
    keyboard *key1;
    keyboard *key2;
};

#endif // SETUP_COMMONSETUP_USERLIST_SETMANUFACTORER_H
