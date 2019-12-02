#ifndef SETUP_TEST_H
#define SETUP_TEST_H

#include <QDialog>
#include <QFile>
#include <QTextStream>
#include "keyboard.h"

namespace Ui {
class Setup_Test;
}

class Setup_Test : public QDialog
{
    Q_OBJECT

public:
    explicit Setup_Test(QWidget *parent = nullptr);
    ~Setup_Test();
signals:
    void setKeyString(QString str);
    void setOncePoint(int x);
    void ClearKey(int x);
public slots:
    void setText1(QString str);
    void setText2(QString str);
    void setText3(QString str);
    void setText4(QString str);
    void setText5(QString str);
    void setText6(QString str);
    void setText7(QString str);
    void setText8(QString str);
    void setText9(QString str);
    void setText10(QString str);
    void setText11(QString str);
    void setText12(QString str);
    void setText13(QString str);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_14_clicked();

private:
    Ui::Setup_Test *ui;
};

#endif // SETUP_TEST_H
