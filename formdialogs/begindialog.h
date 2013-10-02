#ifndef BEGINDIALOG_H
#define BEGINDIALOG_H

#include <QDialog>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE

class BeginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BeginDialog(QWidget *parent = 0);
    ~BeginDialog();
    QString hosNameRead();
    void hostNameWrite(QString str);

private:
    void createForm();

    QLineEdit *hostNameEdit;
};

#endif // BEGINDIALOG_H
