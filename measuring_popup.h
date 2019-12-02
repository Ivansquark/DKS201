#ifndef MEASURING_POPUP_H
#define MEASURING_POPUP_H

#include <QDialog>
#include <QFile>
#include <QTextStream>

namespace Ui {
class Measuring_Popup;
}

class Measuring_Popup : public QDialog
{
    Q_OBJECT

public:
    explicit Measuring_Popup(QWidget *parent = nullptr);
    ~Measuring_Popup();

signals:
    void repeatTest();
    void okTest();

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::Measuring_Popup *ui;
    int DACmin, DACmax;
    double U_Offset_min,U_Offset_max;
    double R2min,R2max,R3min,R3max;
    double C2min,C2max;
    double Imin,Imax;

};

#endif // MEASURING_POPUP_H
