// Окно Настройки-общие настройки-список пользователей***************************

#include "setup_commonsetup_userlist.h"
#include "ui_setup_commonsetup_userlist.h"

Setup_CommonSetup_UserList::Setup_CommonSetup_UserList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setup_CommonSetup_UserList)
{
    ui->setupUi(this);
    setMuster = new Setup_CommonSetup_UserList_SetOperator();
    setManufacturer = new Setup_CommonSetup_UserList_SetManufactorer();
}

Setup_CommonSetup_UserList::~Setup_CommonSetup_UserList()
{
    delete ui;
}

void Setup_CommonSetup_UserList::on_pushButton_4_clicked()
{
    close();
}

void Setup_CommonSetup_UserList::on_pushButton_2_clicked()
{
    setMuster->setModal(true);
    setMuster->showFullScreen();
}

void Setup_CommonSetup_UserList::on_pushButton_3_clicked()
{
    setManufacturer->setModal(true);
    setManufacturer->showFullScreen();
}
