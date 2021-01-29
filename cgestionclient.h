#ifndef CGESTIONCLIENT_H
#define CGESTIONCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>

#include "cmodbustcp.h"

class CGestionClient: public QObject
{
    Q_OBJECT

public:
    CGestionClient(QObject *parent = nullptr, QTcpSocket *client = nullptr);
    ~CGestionClient();

public slots :
    void on_readyRead();
    void on_writeToClients(QString req);


signals :
    void sig_erreur(QString mess);
    void sig_info(QString mess);

private:

    CModbusTcp *_modbus;
    QTcpSocket *_client;
};

#endif // CGESTIONCLIENT_H
