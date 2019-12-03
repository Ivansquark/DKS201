#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QTextCodec>


int main(int argc, char **argv)
{

    QApplication a(argc, argv);

    QString col="QWidget{color: white; background-color: rgb(0,10,30);}"
            "QPushButton{background-color:qradialgradient(cx:0.5, cy:0.5, radius: 1,"
                "fx:0.5, fy:0.5, stop:0 rgba(188, 198, 204, 160), stop:1 rgb(240, 240, 240, 200));border:1px;border-radius:10px;"
                "border:solid grey;"
                "border-style: outset;"
                "border-width: 3px;"
                "border-color: (188, 198, 204, 100);"
                "padding: 6px;}"
                "QPushButton:pressed{border-radius:10px; background: qradialgradient(cx:0.5, cy:0.5, radius: 1,"
                "fx:0.5, fy:0.5, stop:0 rgba(220, 220, 220, 150), stop:1 rgb(250, 250, 250, 250));border:1px;"
                "border:solid grey;"
                "border-style: outset;"
                "border-width: 1px;"
                "border-color: (188, 198, 204, 200);"
                "padding: 6px;}";

    a.setStyleSheet(col);






    MainWindow *w=new MainWindow();




    w->showFullScreen();


    return a.exec();


}
