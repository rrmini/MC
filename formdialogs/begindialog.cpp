#include <QtGui>
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

    QLabel *nameBDLabel = new QLabel(tr("Имя БД"));
    layout->addWidget(nameBDLabel,1,0);

    bdNameEdit = new QLineEdit;
    layout->addWidget(bdNameEdit,1,1);

    QLabel *nameLabel = new QLabel(tr("Имя пользователя"));
    layout->addWidget(nameLabel,2,0);

    nameEdit = new QLineEdit;
    layout->addWidget(nameEdit,2,1);

    QLabel *passw = new QLabel(tr("Пароль"));
    layout->addWidget(passw,3,0);

    passwEdit = new QLineEdit;
    passwEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(passwEdit,3,1);

    buttonBox = new QDialogButtonBox(Qt::Horizontal);
    buttonBox->addButton(QDialogButtonBox::Cancel);
    buttonBox->addButton(QDialogButtonBox::Ok);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(okToContinue()));

    mainLayout->addLayout(layout);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
}

QString BeginDialog::getPasswd(){
    QString passw = passwEdit->text();
    return passw;
}

QString BeginDialog::bdNameRead(){
    QString bdName = bdNameEdit->text();
    return bdName;
}

QString BeginDialog::hosNameRead(){
    QString hostName = hostNameEdit->text();
    return hostName;
}

QString BeginDialog::nameRead(){
    QString name = nameEdit->text();
    return name;
}

void BeginDialog::bdNameWrite(QString str){
    bdNameEdit->setText(str);
}

void BeginDialog::hostNameWrite(QString str){
    hostNameEdit->setText(str);
}

void BeginDialog::nameWrite(QString str){
    nameEdit->setText(str);
}

void BeginDialog::okToContinue(){
    reject();
    return;
}
