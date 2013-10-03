#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QtSql>

class QMdiArea;


class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    bool createDBConnection();
    ~MainWindow();

private:

    void createActions();

    void createMenus();
    void readSettings();
    void writeSettings();

    QAction     *exitAct;
    QMenu   *fileMenu;
    QMdiArea *mdiArea;
    QSettings settings;
    QString bdName;
    QString hostName;
    QString user;
    QString passw;

};

#endif // MAINWINDOW_H
