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
class QMdiSubWindow;
class ConnectionWidget;
class QMdiSubWindow;
class QSignalMapper;
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
    void open(const QString &connectionName);
    void switchDataBase(QAction *action);
    void switchLanguage(QAction *action);
    void showTable(const QString &table);
    void updateDatabaseMenu();
    void setActiveSubWindow(QWidget *window);
    MdiChild *createMdiChild();
    void on_connectionWidget_tableActivated(const QString &table)
    { showTable(table); }
//    void switchTable();

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

    QActionGroup    *languageActionGroup;
    QLabel      *statusLabel;
    MdiChild    *activeMdiChild();
    QMdiSubWindow *findMdiChild(const QString &windowObjectName);
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

signals:
    void statusMessage(const QString &message);
};

class CustomModel: public QSqlTableModel
{
    Q_OBJECT
public:
    explicit CustomModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase()):QSqlTableModel(parent, db) {}
    QVariant data(const QModelIndex &idx, int role) const
    {
        if (role == Qt::BackgroundRole && isDirty(idx))
            return QBrush(QColor(Qt::yellow));
        return QSqlTableModel::data(idx, role);
    }
};
#endif // MAINWINDOW_H
