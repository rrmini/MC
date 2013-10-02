#include <QLabel>
#include <QLayout>
#include "begindialog.h"


BeginDialog::BeginDialog(QWidget *parent) :
    QDialog(parent)

{
    createForm();
}

BeginDialog::~BeginDialog()
{

}

void BeginDialog::createForm(){
    QVBoxLayout *mainLayout = new QVBoxLayout;

    QGridLayout *layout = new QGridLayout;
    QLabel *hostNameLabel = new QLabel(tr("Адрес БД или имя хоста"));
    layout->addWidget(hostNameLabel,0,0);

    hostNameEdit = new QLineEdit;
    layout->addWidget(hostNameEdit,0,1);


    mainLayout->addLayout(layout);
    setLayout(mainLayout);
}

QString BeginDialog::hosNameRead(){
    QString hostName = hostNameEdit->text();
    return hostName;
}

void BeginDialog::hostNameWrite(QString str){
    hostNameEdit->setText(str);
}
