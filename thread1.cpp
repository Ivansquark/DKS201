#include "thread1.h"
#include <QDebug>
#include <QTime>


Thread1::Thread1(QString threadName): name(threadName)
{
}
Thread1::~Thread1()
{
    qDebug()<<"destr"<<name;
    delete this;
}

void Thread1::run()
{
    for (long i = 0; i <= 1e5; i++ )
    {
            qDebug() << name << " " << i <<QTime::currentTime();
    }
}
