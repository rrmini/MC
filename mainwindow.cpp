#include "mainwindow.h"
#include "connectionwidget.h"
#include "formdialogs/databaseconnectiondialog.h"
#include "mdichild.h"
#include <QtWidgets>
#include <QDir>

MainWindow::MainWindow()
{
    readSettings();

    mdiArea = new QMdiArea;
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);

    setWindowTitle("MC-project");
//    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
//            this, SLOT(updateMenus()));

    windowMapper = new QSignalMapper(this);
    connect(windowMapper, SIGNAL(mapped(QWidget*)),
                this, SLOT(setActiveSubWindow(QWidget*)));

    qApp->installTranslator(&appTranslator);
    qApp->installTranslator(&qtTranslator);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createDockWindow();
    retranslate();
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
    if(dialog->isOpen) {
        dbConnectionAct->setIcon(QIcon(":/images/connect24.png"));
        updateDatabaseMenu();
        open();
        connWidget->refresh();
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
    databaseActionGroup = new QActionGroup(this);
    connect(databaseActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(switchDataBase(QAction*)));

    dbConnectionAct = new QAction(this);
    dbConnectionAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    dbConnectionAct->setIcon(QIcon(":/images/disconnect32.png"));
    dbConnectionAct->setStatusTip(tr("database connection"));
    connect(dbConnectionAct, SIGNAL(triggered()), this, SLOT(dbConnection()));

    exitAct = new QAction( this);
    exitAct->setIcon(QIcon(":/images/exit32.png"));
    exitAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    aboutQtAct = new QAction(this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    separatorAct = new QAction(this);
    separatorAct->setSeparator(true);

}

void MainWindow::createDockWindow()
{
    QDockWidget *dock = new QDockWidget(tr("Customers"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    viewMenu->addAction(dock->toggleViewAction());
    connWidget = new ConnectionWidget(this);
    dock->setWidget(connWidget);
    addDockWidget(Qt::RightDockWidgetArea, dock);
}

MdiChild *MainWindow::createMdiChild()
{
    MdiChild *child = new MdiChild;
    mdiArea->addSubWindow(child);

#ifndef QT_NO_CLIPBOARD
//    connect(child, SIGNAL(copyAvailable(bool)),
//            cutAct, SLOT(setEnabled(bool)));
//    connect(child, SIGNAL(copyAvailable(bool)),
//            copyAct, SLOT(setEnabled(bool)));
#endif

    return child;
}

void MainWindow::updateDatabaseMenu()
{
    fileMenu->clear();
    fileMenu->addAction(dbConnectionAct);
    fileMenu->addAction(exitAct);
    fileMenu->addAction(separatorAct);

    QStringList connections = QSqlDatabase::connectionNames();
    separatorAct->setVisible(!connections.isEmpty());

    for(int i=0; i<connections.size(); i++) {
        QString names = connections[i];
        QAction *act =new QAction(names, this);
        act->setCheckable(true);
        act->setData(names);
        fileMenu-> addAction(act);
        databaseActionGroup->addAction(act);
        if(!i) {
            act->setChecked(true);
            switchDataBase(act);//не корректно работет т.к. не понятно на данном этапе какое соединение в данный момент
            // используется. Скорее всего эта проблема решится, когда будут построены окна соответствующие открытм соединениям
            // пока оставляем без изменения
        }
    }
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

        if (locale == MainWindow::locale) {
            QString qmPath = directoryOf("translations").absolutePath();
            appTranslator.load("mainwindow_"+ locale, qmPath);
            qtTranslator.load("qt_" + locale, qmPath);
            action->setChecked(true);
        }
        else if (language =="English") action->setChecked(true);
    }

}

void MainWindow::createMenus(){
    fileMenu = new QMenu(this);
    fileMenu->addAction(dbConnectionAct);
    fileMenu->addAction(exitAct);

    createLanguageMenu();

    viewMenu = new QMenu(this);

    helpMenu = new QMenu(this);
    helpMenu->addAction(aboutQtAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(languageMenu);
    menuBar()->addMenu(viewMenu);
    menuBar()->addMenu(helpMenu);
}

void MainWindow::createStatusBar()
{
    statusLabel = new QLabel(tr("no connect"), this);
    statBar = statusBar();
    statBar->addWidget(statusLabel);
}

void MainWindow::createToolBars()
{
    mineToolBar = addToolBar(tr("Mine"));
    mineToolBar->addAction(dbConnectionAct);
    mineToolBar->addAction(exitAct);

    viewMenu->addAction(mineToolBar->toggleViewAction());
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

void MainWindow::open()
{
  /*  QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        QMdiSubWindow *existing = findMdiChild(fileName);
        if (existing) {
            mdiArea->setActiveSubWindow(existing);
            return;
        }

        MdiChild *child = createMdiChild();
        if (child->loadFile(fileName)) {
            statusBar()->showMessage(tr("File loaded"), 2000);
            child->show();
        } else {
            child->close();
        }
    }*/

    MdiChild *child = createMdiChild();
    child->show();
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
    locale = settings.value("/Settings/locale","").toString();
}

void MainWindow::retranslate(){
    dbConnectionAct ->setText(tr("Connection"));
    exitAct         ->setText(tr("Exit"));
    aboutQtAct      ->setText(tr("About &Qt"));

    fileMenu        ->setTitle(tr("Main"));
    fileMenu        ->setStatusTip(tr("Main"));
    languageMenu    ->setTitle(tr("&Language"));
    viewMenu        ->setTitle(tr("&View"));
    helpMenu        ->setTitle(tr("&Help"));
}

void MainWindow::setActiveSubWindow(QWidget *window)
{
    if (!window)
        return;
    mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

void MainWindow::switchDataBase(QAction *action)
{
    QString name = action->data().toString();
    statusLabel->setText(tr("current database: ") + name);
}

void MainWindow::switchLanguage(QAction *action){
    locale = action->data().toString();
    QString qmPath = directoryOf("translations").absolutePath();
//    QMessageBox::warning(0,"",tr("%1").arg(qmPath));
    appTranslator.load("mainwindow_"+ locale, qmPath);
    qtTranslator.load("qt_" + locale, qmPath);
    retranslate();
}

void MainWindow::writeSettings(){
    QSettings settings("QtProject", "MC");
    settings.setValue("/size",size());
    settings.setValue("/Settings/hostName",hostName);
    settings.setValue("/Settings/bdName",bdName);
    settings.setValue("/Settings/portNumber",portNumber);
    settings.setValue("/Settings/name",user);
    settings.setValue("/Settings/locale",locale);
//    QMessageBox::warning(0,"",tr("%1").arg(locale));
}
