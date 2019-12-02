#ifndef DATE_H
#define DATE_H

#include <QDialog>
#include <QMessageBox>
#include <QDate>
#include <QDebug>

#include <keyboard.h>

namespace Ui {
class Date;
}

class Date : public QDialog
{
    Q_OBJECT

public:
    explicit Date(QWidget *parent = nullptr);
    ~Date();
signals:
    void setKeyString(QString str);
    void setOncePoint(int);
    void sendDate(int,int,int);

public slots:
    void setText1(QString str);
    void setText2(QString str);
    void setText3(QString str);


private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_pushButton_9_clicked();

private:
    Ui::Date *ui;
    QDate date;
};

#endif // DATE_H
