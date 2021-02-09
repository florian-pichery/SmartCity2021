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
    CGestionClient(QObject *parent = nullptr, QTcpSocket *sock = nullptr);
    ~CGestionClient();
    bool isConnected();

private slots :
    void on_readyRead();
    void on_writeToClients(QByteArray req);
    void on_erreur(QString mess);
    void on_info(QString mess);
    void on_disconnected();

signals :
    void sig_erreur(QString mess);
    void sig_info(QString mess);
    void sig_socketDeconnected(QTcpSocket* s);

private:

    char chaine[30];
    //CModbusTcp *_modbus;
    QTcpSocket *_sock;
};

#endif // CGESTIONCLIENT_H
