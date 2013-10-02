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
    ~MainWindow();

private:

    bool createDBConnection();

    void readSettings();
    void writeSettings();

    QMdiArea *mdiArea;
    QSettings settings;

};

#endif // MAINWINDOW_H
