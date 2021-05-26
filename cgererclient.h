#ifndef CGERERCLIENT_H
#define CGERERCLIENT_H

#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QHostAddress>
#include <stdio.h>

#include "cmodbustcp.h"
#include "czdc.h"

class CGererClient : public QObject
{
    Q_OBJECT

public:
    explicit CGererClient(qintptr sd = 0,QObject *parent = nullptr);
    ~CGererClient();

private:

    CZdc *_zdc;
    CModbusTcp *_modbus;
    QTcpSocket *_sock;
    QHostAddress _hostAddress;
    QHostAddress _localAddress;
    quint16 _peerPort, _localPort;
    qintptr _sd;

    int part = 1;

    bool write(int ordre);
    QByteArray read(int ordre);

signals:
    void sig_disconnected();

    void sig_erreur(QString mess);
    void sig_info(QString mess);

public slots:
    void on_goGestionClient();

private slots:
    void on_readyRead();
    void on_writeToClients(QByteArray req);
    void on_disconnected();

    void on_erreur(QString mess);
    void on_info(QString mess);

};

#endif // CGERERCLIENT_H
