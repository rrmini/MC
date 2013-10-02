#ifndef BEGINDIALOG_H
#define BEGINDIALOG_H

#include <QDialog>



class BeginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BeginDialog(QWidget *parent = 0);
    ~BeginDialog();

private:
    void createForm();
};

#endif // BEGINDIALOG_H
