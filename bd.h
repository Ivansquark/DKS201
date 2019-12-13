#ifndef BD_H
#define BD_H

#include <QDialog>
#include <QSql>
#include <QSqlQuery>
#include <QFile>
#include <QDebug>
#include <QMessageBox>

#include "bd_table.h"

namespace Ui {
class BD;
}

class BD : public QDialog
{
    Q_OBJECT

public:
    explicit BD(QWidget *parent = nullptr);
    ~BD();
public:
    void init();

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::BD *ui;

    QString tableName;
};

#endif // BD_H
