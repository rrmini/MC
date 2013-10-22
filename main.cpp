#include <QApplication>
#include "mainwindow.h"
#include "formdialogs/databaseconnectiondialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}
bool DatabaseConnectionDialog::isOpen;
