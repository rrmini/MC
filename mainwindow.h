#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    void switchLanguage(QAction *action);
    void updateWindowMenu();

private:

    QDir directoryOf(const QString &subdir);
    void createActions();

    void createLanguageMenu();
    void createMenus();
    void readSettings();
    void retranslate();
    void writeSettings();

    QAction     *aboutQtAct;
    QAction     *dbConnectionAct;
    QAction     *exitAct;

    QActionGroup *languageActionGroup;
    QMenu       *helpMenu;
    QMenu       *fileMenu;
    QMenu       *languageMenu;
    QMenu       *windowMenu;
    QMdiArea    *mdiArea;
    int         portNumber;
    QString     bdName;
    QString     hostName;
    QString     user;
    QString     passw;

    QTranslator appTranslator;
    QTranslator qtTranslator;
};

#endif // MAINWINDOW_H
