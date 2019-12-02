#include "thirdwin.h"
#include "ui_thirdwin.h"

ThirdWin::ThirdWin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ThirdWin)
{
    ui->setupUi(this);
}

ThirdWin::~ThirdWin()
{
    delete ui;
}

void ThirdWin::on_pushButton_2_clicked()
{
    close();
}
