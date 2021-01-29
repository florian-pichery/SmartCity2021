#ifndef CSERVERTCP_H
#define CSERVERTCP_H


#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QThread>

#include "cgestionclient.h"

class CServerTcp : public QTcpServer
{
    Q_OBJECT

public:
    explicit CServerTcp(QObject *parent = nullptr, quint16 noPort = 2222);
    ~CServerTcp();
    int emettreVersClients(QString mess);

private:
    //variables
    quint16 m_noPort;
    QList<QTcpSocket *> listeClients;//liste des clients connectés

    //objects
    CGestionClient *_client;
    QThread *_gthc;//gestion Thread Client

    //méthodes
    int init();


signals:
    void sig_erreur(QString mess);
    void sig_info(QString mess);

public slots:
    void onNewConnectionClient();
    void onDisconnectedClient();
};

#endif // CSERVERTCP_H
