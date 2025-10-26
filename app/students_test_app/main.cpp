#include <QApplication>
#include <QMainWindow>
#include <QTranslator>
#include <QDir>
#include <QtDebug>

#include "studentsdbmainwindow.h"

int main (int argc, char * argv[])
{
    QApplication * app = new QApplication (argc, argv);

    QMainWindow * rw = new StudentsDbMainWindow;

    rw->show ();

    int retVal = app->exec();

    if(rw)
        delete rw;

    return retVal;
}
