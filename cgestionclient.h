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
    CGestionClient( QTcpSocket *client = nullptr);
    ~CGestionClient();
    bool isConnected();

public slots :
    void on_readyRead();
    void on_writeToClients(QByteArray req);


signals :
    void sig_erreur(QString mess);
    void sig_info(QString mess);

private:

    char ch[30];
    CModbusTcp *_modbus;
    QTcpSocket *_sock;
};

#endif // CGESTIONCLIENT_H
