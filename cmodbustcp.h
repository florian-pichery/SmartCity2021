#ifndef CMODBUSTCP_H
#define CMODBUSTCP_H

#include <QObject>

class CModbusTcp : public QObject
{
    Q_OBJECT
public:
    explicit CModbusTcp(QObject *parent = nullptr);

signals:

public slots:
};

#endif // CMODBUSTCP_H
