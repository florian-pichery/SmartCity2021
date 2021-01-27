#ifndef CSERVERTCP_H
#define CSERVERTCP_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>


class CServerTcp : public QTcpServer
{
    Q_OBJECT

public:
    explicit CServerTcp(QObject *parent = nullptr, quint16 noPort = 2222);
    ~CServerTcp();
    int emettreVersClients(QString mess);

private:
    int init();
    quint16 m_noPort;
    QList<QTcpSocket *> listeClients;  // liste des clients connectés

signals:
    void sig_erreur(QString mess);
    void sig_info(QString mess);
    void sig_newClient(QTcpSocket *client);

public slots:
    void onNewConnectionClient();
    void onDisconnectedClient();
    void onErreurReseau(QAbstractSocket::SocketError err);
    void onReadyReadClient();


};

#endif // CSERVERTCP_H
