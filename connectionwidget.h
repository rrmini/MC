#ifndef CONNECTIONWIDGET_H
#define CONNECTIONWIDGET_H

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(QTreeWidget)
QT_FORWARD_DECLARE_CLASS(QTreeWidgetItem)
QT_FORWARD_DECLARE_CLASS(QSqlDatabase)

class ConnectionWidget : public QWidget
{
    Q_OBJECT
public:
    ConnectionWidget(QWidget *parent = 0);

    QSqlDatabase currentDatabase() const;
    QTreeWidget *tree;

signals:
    void tableActivated(const QString &table);
    void metaDataRequested(const QString &tableName);
//    void tabActiv(const QString &table);

public slots:
    void refresh();
    void showMetaData();
    void on_tree_itemActivated(QTreeWidgetItem *item, int column);
    void on_tree_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

private:
    void setActive(QTreeWidgetItem *);


    QAction *metaDataAction;
    QString activeDb;
};

#endif // CONNECTIONWIDGET_H
