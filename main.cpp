#include "mainwindow.h"
#include "addressbook.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    //return a.exec();

    AddressBook addressbook;
    addressbook.show();

    return a.exec();
}
