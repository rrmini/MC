#include <QtWidgets>
#include "widget.h"

Widget::Widget(QSqlQueryModel *model, QWidget *parent) :
    QWidget(parent)
{
    tableView = new QTableView;
    tableView->setModel(model);

    QVBoxLayout     *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tableView);

    this->setLayout(mainLayout);
}
