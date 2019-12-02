#ifndef SECOND_H
#define SECOND_H

#include <QDialog>
#include "third.h"

namespace Ui
{
class Second;
}

class Second : public QDialog
{
    Q_OBJECT

public:
    explicit Second(QWidget *parent = nullptr);
    ~Second();
public slots:
    void recieve_msg(QString str);

signals:
    void sendMessage1(QString str);

private slots:
    void on_Back_clicked();

    void on_NewWin1_clicked();

private:
    Ui::Second *ui;
    Third *third;

};

#endif // SECOND_H
