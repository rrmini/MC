#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QtSql>

//class QMdiArea;
class MdiChild;
QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QMenu;
class QMdiArea;
class ConnectionWidget;
//class QMdiSubWindow;
//class QSignalMapper;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void dbConnection();
    void open();
    void switchDataBase(QAction *action);
    void switchLanguage(QAction *action);
    void updateDatabaseMenu();
    void setActiveSubWindow(QWidget *window);
    MdiChild *createMdiChild();

private:

    QDir directoryOf(const QString &subdir);
    void createActions();

    void createDockWindow();
    void createLanguageMenu();
    void createMenus();
    void createStatusBar();
    void createToolBars();
    void readSettings();
    void retranslate();
    void writeSettings();

    ConnectionWidget *connWidget;
    QAction     *aboutQtAct;
    QAction     *dbConnectionAct;
    QAction     *exitAct;
    QAction     *separatorAct;

    QActionGroup    *databaseActionGroup;
    QActionGroup    *languageActionGroup;
    QLabel      *statusLabel;
//    QLineEdit   *editLine;
    QMenu       *helpMenu;
    QMenu       *fileMenu;
    QMenu       *languageMenu;
    QMenu       *viewMenu;
    QMdiArea    *mdiArea;
    int         portNumber;
    QSignalMapper *windowMapper;
    QStatusBar  *statBar;
    QString     bdName;
    QString     hostName;
    QString     user;
    QString     passw;
    QString     locale;

    QToolBar    *mineToolBar;

    QTranslator appTranslator;
    QTranslator qtTranslator;
};

#endif // MAINWINDOW_H
