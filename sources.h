#ifndef SOURCES_H
#define SOURCES_H

#include <QDialog>
#include <QFile>
#include <QtSql/QSql>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QDate>

#include "add_source.h"


namespace Ui {
class Sources;
}

class Sources : public QDialog
{
    Q_OBJECT

public:
    explicit Sources(QWidget *parent = nullptr);
    ~Sources();

    void init();

signals:
    void setCS(QString);
    void constr(QString);

public slots:
    void newSourceList();


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_comboBox_activated();

    void on_pushButton_4_clicked();

private:
    Ui::Sources *ui;

    double doseRateCurrentCS;
    double doseRateVerificationCS,activityFabricationCS;
    QDate dateVerificationCS;
    double half_lifeCS;
    QString doseRateDimension;
    double doseRateFull,activityCurrentCS;


};

#endif // SOURCES_H
