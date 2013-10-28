#ifndef CONNECTIONWIDGET_H
#define CONNECTIONWIDGET_H

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(QTreeWidget)
QT_FORWARD_DECLARE_CLASS(QTreeWidgetItem)

class ConnectionWidget : public QWidget
{
    Q_OBJECT
public:
    ConnectionWidget(QWidget *parent = 0);

signals:
    void metaDataRequested(const QString &tableName);

public slots:
    void refresh();
    void showMetaData();

private:
    void setActive(QTreeWidgetItem *);

    QTreeWidget *tree;
    QAction *metaDataAction;
    QString activeDb;
};

#endif // CONNECTIONWIDGET_H
