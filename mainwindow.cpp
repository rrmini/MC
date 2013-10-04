#include "mainwindow.h"
#include "formdialogs/begindialog.h"

#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QMdiArea>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), settings("OCS","MC")
{
    readSettings();
    BeginDialog dialog;
    dialog.hostNameWrite(hostName);
    dialog.bdNameWrite(bdName);
    dialog.nameWrite(user);

    dialog.exec();
    hostName  =dialog.hosNameRead();
    bdName = dialog.bdNameRead();
    user = dialog.nameRead();
    passw = dialog.getPasswd();

    mdiArea = new QMdiArea;
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);

//    setWindowTitle(hostName);
//    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
//            this, SLOT(updateMenus()));

    createActions();
    createMenus();
}

MainWindow::~MainWindow()
{
    writeSettings();
}

void MainWindow::createActions(){
    exitAct = new QAction(trUtf8("Выход"), this);
    exitAct->setIcon(QIcon(":/images/door_out.png"));
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(trUtf8("Выход из программы"));
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
}

bool MainWindow::createDBConnection()
{
    QSqlDatabase db;
        db = QSqlDatabase::addDatabase("QMYSQL");
        if (hostName == tr("")){
            QMessageBox::warning(this, "", trUtf8("Не указан адрес БД"));
            return false;
        }
        db.setHostName(hostName);
        if (bdName == tr("")){
            QMessageBox::warning(this, "", tr("Не указано имя базы данных"));
            return false;
        }
        db.setDatabaseName(bdName);//mc
        if (user == tr("")){
            QMessageBox::warning(this, "", tr("Не указано имя пользователя"));
            return false;
        }
        db.setUserName(user);
        db.setPassword(passw);//паровоз
        if(!db.open()){
            QMessageBox::warning(0, "",db.lastError().text());
            return false;
        }
        return true;
}

void MainWindow::createMenus(){
    fileMenu = menuBar()->addMenu(trUtf8("Файл"));
    fileMenu->addAction(exitAct);
}

void MainWindow::readSettings(){
    settings.beginGroup("/Settings");
    QSize size = settings.value("/size",sizeHint()).toSize();
    resize(size);
    hostName = settings.value("/Settings/hostName","").toString();
    bdName = settings.value("/Settings/bdName","").toString();
    user = settings.value("/Settings/name","").toString();
    if (user == tr("")) user = tr("*****") ;
    settings.endGroup();
}

void MainWindow::writeSettings(){
    settings.beginGroup("/Settings");
    settings.setValue("/size",size());
    settings.setValue("/Settings/hostName",hostName);
    settings.setValue("/Settings/bdName",bdName);
    settings.setValue("/Settings/name",user);
    settings.endGroup();
}
