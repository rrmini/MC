#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtSql>
#include <QTableView>

//class QTableView;
class Widget : public QWidget
{
    Q_OBJECT
public:
    /*explicit*/ Widget(QSqlQueryModel *model,QWidget *parent = 0);

signals:

public slots:

private:
    QTableView      *tableView;
};

#endif // WIDGET_H
