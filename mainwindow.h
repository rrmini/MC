#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QtSql>

class QMdiArea;
QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QMenu;
class QMdiArea;
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
    void switchDataBase(QAction *action);
    void switchLanguage(QAction *action);
    void updateDatabaseMenu();

private:

    QDir directoryOf(const QString &subdir);
    void createActions();


    void createLanguageMenu();
    void createMenus();
    void createStatusBar();
    void createToolBars();
    void readSettings();
    void retranslate();
    void writeSettings();

    QAction     *aboutQtAct;
    QAction     *dbConnectionAct;
    QAction     *exitAct;
    QAction     *separatorAct;

    QActionGroup    *databaseActionGroup;
    QActionGroup    *languageActionGroup;
    QLabel      *statusLabel;
    QLineEdit   *editLine;
    QMenu       *helpMenu;
    QMenu       *fileMenu;
    QMenu       *languageMenu;
    QMdiArea    *mdiArea;
    int         portNumber;
    QStatusBar  *statBar;
    QString     bdName;
    QString     hostName;
    QString     user;
    QString     passw;
    QString     locale;

    QToolBar    *mineToolBar;
    QToolBar    *rightToolBar;

    QTranslator appTranslator;
    QTranslator qtTranslator;
};

#endif // MAINWINDOW_H
