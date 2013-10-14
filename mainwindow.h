#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>

class QMdiArea;
QT_BEGIN_NAMESPACE
class QAction;
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
    bool createDBConnection();
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void dbConnection();

private:

    void createActions();

    void createMenus();
    void readSettings();
    void writeSettings();

    QAction     *dbConnectionAct;
    QAction     *exitAct;
    QMenu       *fileMenu;
    QMdiArea    *mdiArea;
    int         portNumber;
    QString     bdName;
    QString     hostName;
    QString     user;
    QString     passw;

};

#endif // MAINWINDOW_H
