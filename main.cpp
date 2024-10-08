#include <QApplication>
#include "mainwindow.h"
#include "fileio.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("MySimpleAlarm");
    a.setOrganizationName("NSBM");
    MainWindow w;
    w.show();
    return a.exec();
}
