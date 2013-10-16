#include "mainwindow.h"
#include "formdialogs/begindialog.h"
#include "formdialogs/databaseconnectiondialog.h"

//#include <QTranslator>
#include <QtWidgets>

MainWindow::MainWindow()
{
    readSettings();
  /*  BeginDialog dialog;
    dialog.hostNameWrite(hostName);
    dialog.bdNameWrite(bdName);
    dialog.nameWrite(user);

    dialog.exec();
    hostName  =dialog.hosNameRead();
    bdName = dialog.bdNameRead();
    user = dialog.nameRead();
    passw = dialog.getPasswd();*/

    mdiArea = new QMdiArea;
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);

    setWindowTitle(hostName);
//    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
//            this, SLOT(updateMenus()));

    createActions();
    createMenus();
}

MainWindow::~MainWindow()
{
    writeSettings();
}

void MainWindow::dbConnection(){
    DatabaseConnectionDialog* dialog = new DatabaseConnectionDialog(this);
    dialog->setDatabaseHostName(hostName);
    dialog->setDatabaseName(bdName);
    dialog->setDatabasePortNumber(portNumber);
    dialog->setDatabaseUsername(user);
    dialog->exec();
    bdName = dialog->dbName();
    portNumber = dialog->portNumber();
    hostName = dialog->hostName();
    user = dialog->userName();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    mdiArea->closeAllSubWindows();
    if (mdiArea->currentSubWindow()) {
        event->ignore();
    } else {
        writeSettings();
        event->accept();
    }
}

void MainWindow::createActions(){
    dbConnectionAct = new QAction(tr("Connection"),this);
    dbConnectionAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    connect(dbConnectionAct, SIGNAL(triggered()), this, SLOT(dbConnection()));

    exitAct = new QAction(tr("Exit"), this);
//    exitAct->setIcon(QIcon(":/images/door_out.png"));
    exitAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    languageRuAct = new QAction(tr("RUS"),this);
    connect(languageRuAct, SIGNAL(triggered()), SLOT(switchLanguageRu()));
}

bool MainWindow::createDBConnection()
{
    QSqlDatabase db;
        db = QSqlDatabase::addDatabase("QMYSQL");
        if (hostName == tr("")){
            QMessageBox::warning(this, "", trUtf8("No address database"));
            return false;
        }
        db.setHostName(hostName);
        if (bdName == tr("")){
            QMessageBox::warning(this, "", tr("Not Specified database name"));
            return false;
        }
        db.setDatabaseName(bdName);//mc
        if (user == tr("")){
            QMessageBox::warning(this, "", tr("Not a user name"));
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
    fileMenu = menuBar()->addMenu(tr("Main"));
    fileMenu->addAction(dbConnectionAct);
    fileMenu->addAction(exitAct);

    languageMenu = menuBar()->addMenu(tr("Language"));
    languageMenu->addAction(languageRuAct);
}

void MainWindow::readSettings(){
    QSettings settings("QtProject", "MC");
    QSize size = settings.value("/size",sizeHint()).toSize();
    resize(size);
    hostName = settings.value("/Settings/hostName","").toString();
    bdName = settings.value("/Settings/bdName","").toString();
    portNumber = settings.value("/Settings/portNumber",3306).toInt();
    user = settings.value("/Settings/name","").toString();
    if (user == tr("")) user = tr("*****") ;
}

void MainWindow::switchLanguageRu(){
    QTranslator translator;
    if(translator.load("/translations/mainwindow_ru.qm"))
        QMessageBox::warning(this,"",tr("Ok"));
    qApp->installTranslator(&translator);
}

void MainWindow::writeSettings(){
    QSettings settings("QtProject", "MC");
    settings.setValue("/size",size());
    settings.setValue("/Settings/hostName",hostName);
    settings.setValue("/Settings/bdName",bdName);
    settings.setValue("/Settings/portNumber",portNumber);
    settings.setValue("/Settings/name",user);
}
