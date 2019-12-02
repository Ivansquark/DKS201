#include "chamber_config.h"
#include "ui_chamber_config.h"

Chamber_config::Chamber_config(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Chamber_config)
{
    ui->setupUi(this);
}

Chamber_config::~Chamber_config()
{
    delete ui;
}

void Chamber_config::on_pushButton_2_clicked()
{
    close();
}
