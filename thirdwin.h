#ifndef THIRDWIN_H
#define THIRDWIN_H

#include <QWidget>

namespace Ui {
class ThirdWin;
}

class ThirdWin : public QWidget
{
    Q_OBJECT

public:
    explicit ThirdWin(QWidget *parent = nullptr);
    ~ThirdWin();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::ThirdWin *ui;
};

#endif // THIRDWIN_H
