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

MdiChild *MainWindow::activeMdiChild()
{
    if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
        return qobject_cast<MdiChild *>(activeSubWindow->widget());
    return 0;
}

void MainWindow::dbConnection(){
    DatabaseConnectionDialog* dialog = new DatabaseConnectionDialog(this);
    dialog->setDatabaseHostName(hostName);
    dialog->setDatabaseName(bdName);
    dialog->setDatabasePortNumber(portNumber);
    dialog->setDatabaseUsername(user);
    dialog->exec();
//    QMessageBox::warning(this, "", tr("%1").arg(dialog->isOpen));
    if(dialog->isOpen) {
        dbConnectionAct->setIcon(QIcon(":/images/connect24.png"));

        open(dialog->hostName()+" "+dialog->dbName());
        updateDatabaseMenu();
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

void MainWindow::createActions()
{
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
//    child->setModel();
    mdiArea->addSubWindow(child);

#ifndef QT_NO_CLIPBOARD
//    connect(child, SIGNAL(copyAvailable(bool)),
//            cutAct, SLOT(setEnabled(bool)));
//    connect(child, SIGNAL(copyAvailable(bool)),
//            copyAct, SLOT(setEnabled(bool)));
#endif

    return child;
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
    connect(fileMenu, SIGNAL(aboutToShow()), this, SLOT(updateDatabaseMenu()));

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

void MainWindow::open(const QString &connectionName)
{
    QMdiSubWindow *existing = findMdiChild(connectionName);
    if (existing) {
        mdiArea->setActiveSubWindow(existing);
        return;
    }

    MdiChild *child = createMdiChild();
    child->setObjectName(connectionName);
    child->setWindowTitle(connectionName);
    child->show();
}

QMdiSubWindow *MainWindow::findMdiChild(const QString &windowObjectName)
{
    foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
        MdiChild *mdiChild = qobject_cast<MdiChild *>(window->widget());
        if (mdiChild->objectName() == windowObjectName)
            return window;
    }
    return 0;
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
    open(name);
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

void MainWindow::updateDatabaseMenu()
{
    fileMenu->clear();
    fileMenu->addAction(dbConnectionAct);
    fileMenu->addAction(exitAct);
    fileMenu->addAction(separatorAct);

    QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
    separatorAct->setVisible(!windows.isEmpty());

    for (int i = 0; i < windows.size(); ++i) {
        MdiChild *child = qobject_cast<MdiChild *>(windows.at(i)->widget());

        QString text;
//        if (i < 9) {
//            text = tr("&%1 %2").arg(i + 1)
//                               .arg(child->objectName());
//        } else {
//            text = tr("%1 %2").arg(i + 1)
//                              .arg(child->objectName());
//        }
        text = child->objectName();
        QAction *action  = fileMenu->addAction(text);
        action->setCheckable(true);
        action ->setChecked(child == activeMdiChild());
        connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
        windowMapper->setMapping(action, windows.at(i));
    }
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
