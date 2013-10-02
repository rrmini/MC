#include <QtGui>
#include "mainwindow.h"
#include "formdialogs/begindialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), settings("OCS","MC")
{
    readSettings();
    BeginDialog dialog;
    dialog.exec();

    if (createDBConnection())
        QMessageBox::warning(this, "", trUtf8("Yes соединения с БД"));

    mdiArea = new QMdiArea;
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);
//    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
//            this, SLOT(updateMenus()));
}

MainWindow::~MainWindow()
{
    writeSettings();
}

bool MainWindow::createDBConnection()
{
    QSqlDatabase db;
        db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("localhost");
        db.setDatabaseName("mc");
        db.setUserName("roman");
        db.setPassword("gfhjdjp");
        if(!db.open()){
            QMessageBox::warning(0, "",db.lastError().text());
            return false;
        }
        return true;
}

void MainWindow::readSettings(){
    settings.beginGroup("/Settings");
    QSize size = settings.value("/size",sizeHint()).toSize();
    resize(size);
    settings.endGroup();
}

void MainWindow::writeSettings(){
    settings.beginGroup("/Settings");
    settings.setValue("/size",size());
    settings.endGroup();
}
