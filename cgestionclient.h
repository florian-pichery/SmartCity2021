#ifndef CGESTIONCLIENT_H
#define CGESTIONCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include "cmodbustcp.h"

class CGestionClient: public QObject
{
    Q_OBJECT

public:
    CGestionClient(QObject *parent = nullptr, QTcpSocket *client = nullptr);

private slots :
    void on_readyRead();

signals:

    void sig_erreur(QString mess);
    void sig_info(QString mess);

private:

    CModbusTcp *_modbus;
    QTcpSocket *_client;
};

#endif // CGESTIONCLIENT_H
