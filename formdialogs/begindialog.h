#ifndef BEGINDIALOG_H
#define BEGINDIALOG_H

#include <QDialog>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QLineEdit;
QT_END_NAMESPACE

class BeginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BeginDialog(QWidget *parent = 0);
    ~BeginDialog();
    QString bdNameRead();
    QString hosNameRead();
    QString nameRead();
    QString getPasswd();
    void bdNameWrite(QString str);
    void hostNameWrite(QString str);
    void nameWrite(QString str);


private slots:
    void okToContinue();

private:
    void createForm();

    QDialogButtonBox *buttonBox;
    QLineEdit *bdNameEdit;
    QLineEdit *hostNameEdit;
    QLineEdit *nameEdit;
    QLineEdit *passwEdit;
};

#endif // BEGINDIALOG_H
