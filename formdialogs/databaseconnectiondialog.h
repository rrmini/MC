#ifndef DATABASEDIALOG_H
#define DATABASEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSqlDatabase>
#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
#include <QPushButton>
#include <QGroupBox>

class DatabaseConnectionDialog : public QDialog
{
    Q_OBJECT

private:

    QLabel* labelDatabaseDriverName;
    QLabel* labelDatabasePort;
    QLabel* labelDatabaseName;
    QLabel* labelDatabaseHostName;
    QLabel* labelDatabaseUsername;
    QLabel* labelDatabasePassword;
    QLabel* labelDatabaseURL;
    QLineEdit* editDatabaseName;
    QLineEdit* editDatabaseHostName;
    QSpinBox* spinBoxDatabasePort;
    QLineEdit* editDatabaseUsername;
    QLineEdit* editDatabasePassword;
    QComboBox* comboDatabaseDriverName;
    QDialogButtonBox* buttons;
    QGroupBox* gridGroupBox;
    QGroupBox* urlGroupBox;

    void setUpGUI();
    void createGridGroupBox();
    void createUrlGroupBox();
    void createWidgets();
    void findAvailableDrivers();
    void doDatabaseConnection();

public:
    explicit DatabaseConnectionDialog(QWidget *parent = 0);
    ~DatabaseConnectionDialog();

    bool isOpen;
    void setDatabaseName( const QString& dbName );
    void setDatabasePortNumber( int& portNumber );
    void setDatabaseHostName( const QString& hostname );
    void setDatabaseUsername( const QString& username );
    void setDatabaseDriverName( const QString& drvName );
    void setDatabasePassword( const QString& pwd );
    int portNumber();
    const QString dbName();
    const QString hostName();
    const QString userName();

    /*!
      * Performs a check against the user data and enables/disables
      * the connect button depending on the form fill status.
      *\return true if the data allows a database connection
      */
    bool checkFormData();

    /*!
      * Performs the database connection or prompt the user
      * showing this dialog in the case data is not completed
      * or should not perform the autoconnection.
      * \param autoConnect if set to true tries to perform an automatic
      * connection to the database, if the data is complete, or prompt the user
      * for missing data. If set to false, simply shows the dialog and waits.
      */
    void run( bool autoConnect );


signals:

    /*!
      * Passes the database connection in the case the connection
      * is succesful.
      * \param databaseConnection the connection object
      */
    void databaseConnect( QSqlDatabase& databaseConnection );

public slots:

    /*!
      * Checks if the user has entered enough data to
      * try a database connection.
      */
    bool slotCheckFormData();


    /*!
      * Performs the database connection.
      */
    void slotPerformConnection();

};

#endif // DATABASEDIALOG_H
