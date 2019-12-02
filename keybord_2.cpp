#include "keybord_2.h"
#include "ui_keybord_2.h"

Keybord_2::Keybord_2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Keybord_2)
{
    ui->setupUi(this);
}

Keybord_2::~Keybord_2()
{
    delete ui;
}

void Keybord_2::on_b1_clicked()
{
    line+=ui->b1->text();
    ui->lineEdit->setText(line);
}
void Keybord_2::on_b2_clicked()
{
    line+=ui->b2->text();
    ui->lineEdit->setText(line);
}
void Keybord_2::on_b3_clicked()
{
    line+=ui->b3->text();
    ui->lineEdit->setText(line);
}
void Keybord_2::on_b4_clicked()
{
    line+=ui->b4->text();
    ui->lineEdit->setText(line);
}
void Keybord_2::on_b5_clicked()
{
    line+=ui->b5->text();
    ui->lineEdit->setText(line);
}
void Keybord_2::on_b6_clicked()
{
    line+=ui->b6->text();
    ui->lineEdit->setText(line);
}
void Keybord_2::on_b7_clicked()
{
    line+=ui->b1->text();
    ui->lineEdit->setText(line);
}
void Keybord_2::on_b8_clicked()
{
    line+=ui->b8->text();
    ui->lineEdit->setText(line);
}
void Keybord_2::on_b9_clicked()
{
    line+=ui->b9->text();
    ui->lineEdit->setText(line);
}
void Keybord_2::on_b10_clicked()
{
    line+=ui->b10->text();
    ui->lineEdit->setText(line);
}
void Keybord_2::on_b_mmm_2_clicked()
{
    line+=ui->b_mmm_2->text();
    ui->lineEdit->setText(line);
}
void Keybord_2::on_b_backspace_clicked()
{
    line.chop(1);
    ui->lineEdit->setText(line);
}

void Keybord_2::on_b_enter_clicked()
{
    emit setFigures(ui->lineEdit->text());
    close();
}
