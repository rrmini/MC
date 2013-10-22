#include "mainwindow.h"
#include "formdialogs/databaseconnectiondialog.h"

#include <QtWidgets>
#include <QDir>

MainWindow::MainWindow()
{
    readSettings();

    mdiArea = new QMdiArea;
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);

    setWindowTitle(hostName);
//    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
//            this, SLOT(updateMenus()));
    qApp->installTranslator(&appTranslator);
    qApp->installTranslator(&qtTranslator);

    createActions();
    createMenus();
    retranslate();
}

MainWindow::~MainWindow()
{
    writeSettings();
}

void MainWindow::dbConnection(){
    DatabaseConnectionDialog* dialog = new DatabaseConnectionDialog(this/*, &db*/);
    dialog->setDatabaseHostName(hostName);
    dialog->setDatabaseName(bdName);
    dialog->setDatabasePortNumber(portNumber);
    dialog->setDatabaseUsername(user);
    dialog->exec();
    if(dialog->isOpen) {
        dbConnectionAct->setIcon(QIcon(":/images/connect32.png"));
//        dbConnectionAct->setEnabled(false);
    }
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
    dbConnectionAct = new QAction(this);
    dbConnectionAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    dbConnectionAct->setIcon(QIcon(":/images/disconnect32.png"));
    connect(dbConnectionAct, SIGNAL(triggered()), this, SLOT(dbConnection()));

    exitAct = new QAction( this);
//    exitAct->setIcon(QIcon(":/images/door_out.png"));
    exitAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    aboutQtAct = new QAction(this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

}

void MainWindow::createLanguageMenu(){
    languageMenu = new QMenu(this);

    languageActionGroup = new QActionGroup(this);
    connect(languageActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(switchLanguage(QAction*)));

    QDir qmDir = directoryOf("translations");
    QStringList fileNames = qmDir.entryList(QStringList("mainwindow_*.qm"));
//    QMessageBox::warning(0,"",tr("%1").arg(qmDir.absolutePath()));
/*
Внимательно смотрим каталог сборки приложения и туда не забудем поместить нашу папочку
"translations" с файлами переводов *.qm в том числе и файлы из <QTDIR>/translations/
*/

    for(int i = 0; i < fileNames.size(); ++i){
        QString locale = fileNames[i];
        locale.remove(0, locale.indexOf('_') + 1);
        locale.chop(3);

        QTranslator translator;
        translator.load(fileNames[i], qmDir.absolutePath());
        QString language = translator.translate("MainWindow", "English");

        QAction *action = new QAction(tr("&%1 %2").arg(i + 1).arg(language), this);
        action->setCheckable(true);
        action->setData(locale);

        languageMenu->addAction(action);
        languageActionGroup->addAction(action);

        if (language == "English") action->setChecked(true);
    }
}

void MainWindow::createMenus(){
    fileMenu = new QMenu(this);
    fileMenu->addAction(dbConnectionAct);
    fileMenu->addAction(exitAct);

    windowMenu = new QMenu(this);
//    windowMenu = menuBar()->addMenu(tr("&Window"));
    updateWindowMenu();
    connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

    createLanguageMenu();

    helpMenu = new QMenu(this);
    helpMenu->addAction(aboutQtAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(windowMenu);
    menuBar()->addMenu(languageMenu);
    menuBar()->addMenu(helpMenu);
}

QDir MainWindow::directoryOf(const QString &subdir){
    QDir dir(QApplication::applicationDirPath());

#if defined(Q_OS_WIN)
    if (dir.dirName.toLower() == "debug"
            || dirName().toLower() == "release")
        dir.cdUp();
#elif defined(Q_OS_MAC)
    if (dir.dirName() == "MacOS") {
        dir.cdUp();
        dir.cdUp();
        dir.cdUp();
    }
#endif
    dir.cd(subdir);
    return dir;
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

void MainWindow::retranslate(){
    dbConnectionAct ->setText(tr("Connection"));
    exitAct         ->setText(tr("Exit"));
    aboutQtAct      ->setText(tr("About &Qt"));

    fileMenu        ->setTitle(tr("Main"));
    windowMenu      ->setTitle(tr("&Window"));
    languageMenu    ->setTitle(tr("&Language"));
    helpMenu        ->setTitle(tr("&Help"));
}

void MainWindow::switchLanguage(QAction *action){
    QString locale = action->data().toString();
    QString qmPath = directoryOf("translations").absolutePath();
//    QMessageBox::warning(0,"",tr("%1").arg(qmPath));
    appTranslator.load("mainwindow_"+ locale, qmPath);
    qtTranslator.load("qt_" + locale, qmPath);
    retranslate();
}

void MainWindow::updateWindowMenu(){
    windowMenu->clear();
    QStringList connections = QSqlDatabase::connectionNames();
    for(int i=0; i<connections.size(); i++) {
        QString names = connections[i];
        QAction *act =new QAction(names, this);
        windowMenu-> addAction(act);
    }
}

void MainWindow::writeSettings(){
    QSettings settings("QtProject", "MC");
    settings.setValue("/size",size());
    settings.setValue("/Settings/hostName",hostName);
    settings.setValue("/Settings/bdName",bdName);
    settings.setValue("/Settings/portNumber",portNumber);
    settings.setValue("/Settings/name",user);
}
