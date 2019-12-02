#ifndef CHAMBERS_H
#define CHAMBERS_H

#include <QDialog>

#include "add_chamber.h"
#include "measuring_parameters.h"

#include <QSql>
#include <QSqlDriver>
//#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QSqlField>
#include <QSqlError>


namespace Ui {
class Chambers;
}

class Chambers : public QDialog
{
    Q_OBJECT

public:
    explicit Chambers(QWidget *parent = nullptr);
    ~Chambers();
signals:
    void constr(QString num);
    void butOk(QString numChamber);
    void updateData();
    void updateMesuaring_parameters();
    void okChambers(); // сигнал в MainWindow об открытии окна впервые после теста


public slots:
   void newCombo();
   void firstTimeFlag();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_comboBox_activated();


    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::Chambers *ui;
    int F;

};

#endif // CHAMBERS_H
