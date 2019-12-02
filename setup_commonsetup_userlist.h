// Окно Настройки-общие настройки-список пользователей***************************

#ifndef SETUP_COMMONSETUP_USERLIST_H
#define SETUP_COMMONSETUP_USERLIST_H

#include <QDialog>
#include "setup_commonsetup_userlist_setoperator.h"
#include "setup_commonsetup_userlist_setmanufactorer.h"

namespace Ui {
class Setup_CommonSetup_UserList;
}

class Setup_CommonSetup_UserList : public QDialog
{
    Q_OBJECT

public:
    explicit Setup_CommonSetup_UserList(QWidget *parent = nullptr);
    ~Setup_CommonSetup_UserList();

private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Setup_CommonSetup_UserList *ui;
    Setup_CommonSetup_UserList_SetOperator *setMuster;
    Setup_CommonSetup_UserList_SetManufactorer *setManufacturer;
};

#endif // SETUP_COMMONSETUP_USERLIST_H
