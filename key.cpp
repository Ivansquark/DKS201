#include "key.h"
#include "ui_key.h"

Key::Key(QWidget *parent) :    QDialog(parent), ui(new Ui::Key)
{
    ui->setupUi(this);
}

Key::~Key()
{
    delete ui;
}
