#ifndef KEY_H
#define KEY_H

#include <QDialog>

namespace Ui {
class Key;
}

class Key : public QDialog
{
    Q_OBJECT

public:
    explicit Key(QWidget *parent = nullptr);
    ~Key();

private:
    Ui::Key *ui;
};

#endif // KEY_H
