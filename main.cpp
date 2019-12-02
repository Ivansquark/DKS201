#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QTextCodec>


int main(int argc, char **argv)
{

    QApplication a(argc, argv);
    MainWindow *w=new MainWindow();
    w->showFullScreen();


    return a.exec();


}
