#ifndef MDICHILD_H
#define MDICHILD_H

#include <QTableView>

class MdiChild : public QTableView
{
    Q_OBJECT
public:
    /*explicit*/ MdiChild();

signals:

public slots:
private:
    bool isUntitled;
};

#endif // MDICHILD_H
