#ifndef THREAD1_H
#define THREAD1_H

#include <QThread>

class Thread1: public QThread
{
public:
    explicit Thread1(QString threadName);
    ~Thread1();

    void run();
private:
    QString name;
};

#endif // THREAD1_H
